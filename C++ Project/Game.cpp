#include "Game.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

Game::Game() : grigorakis(nullptr), asimenia(nullptr), currentTurn(0), 
               gameWon(false), gameLost(false), wallsRemoved(false) {
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
}

Game::~Game() {
    cleanupNCurses();
    delete grigorakis;
    delete asimenia;
}

bool Game::initialize(const string& mapFile) {
    // Φόρτωση λαβύρινθου
    if (!maze.loadFromFile(mapFile)) {
        cerr << "Failed to load maze from " << mapFile << endl;
        return false;
    }
    
    // Δημιουργία ηρώων
    grigorakis = new Grigorakis();
    asimenia = new Asimenia();
    
    // Τοποθέτηση ηρώων
    if (!placeHeroes()) {
        cerr << "Failed to place heroes" << endl;
        return false;
    }
    
    // Τοποθέτηση αντικειμένων
    if (!placeObjects()) {
        cerr << "Failed to place objects" << endl;
        return false;
    }
    
    // Αρχικοποίηση ncurses
    initializeNCurses();
    
    return true;
}

void Game::run() {
    gameLoop();
}

void Game::initializeNCurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(200); // 200ms timeout για getch()
    
    if (has_colors()) {
        start_color();
        initializeColors();
    }
    
    clear();
}

void Game::cleanupNCurses() {
    if (isendwin() == FALSE) {
        endwin();
    }
}

void Game::initializeColors() {
    init_pair(COLOR_WALL, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_GRIGORAKIS, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_ASIMENIA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_KEY, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_TRAP, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_CAGE, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_LADDER, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_UI, COLOR_CYAN, COLOR_BLACK);
}

bool Game::placeHeroes() {
    vector<Position> validPositions = maze.getValidPositions();
    
    if (validPositions.size() < 2) {
        return false;
    }
    
    // Τοποθέτηση Γρηγοράκη
    uniform_int_distribution<int> dist(0, static_cast<int>(validPositions.size()) - 1);
    int grigorakisIndex = dist(rng);
    Position grigorakisPos = validPositions[grigorakisIndex];
    grigorakis->setPosition(grigorakisPos);
    
    // Τοποθέτηση Ασημένιας (τουλάχιστον 7 τετράγωνα μακριά)
    vector<Position> validAsimensiaPositions;
    for (const Position& pos : validPositions) {
        int horizontalDistance = abs(pos.getX() - grigorakisPos.getX());
        int verticalDistance = abs(pos.getY() - grigorakisPos.getY());
        
        if (horizontalDistance >= 7 || verticalDistance >= 7) {
            validAsimensiaPositions.push_back(pos);
        }
    }
    
    if (validAsimensiaPositions.empty()) {
        return false;
    }
    
    uniform_int_distribution<int> asimeniaDist(0, static_cast<int>(validAsimensiaPositions.size()) - 1);
    int asimensiaIndex = asimeniaDist(rng);
    Position asimensiaPos = validAsimensiaPositions[asimensiaIndex];
    asimenia->setPosition(asimensiaPos);
    
    return true;
}

bool Game::placeObjects() {
    vector<Position> validPositions = maze.getValidPositions();
    
    // Αφαίρεση θέσεων ηρώων και σκάλας
    validPositions.erase(
        remove_if(validPositions.begin(), validPositions.end(),
            [this](const Position& pos) {
                return pos == grigorakis->getPosition() || 
                       pos == asimenia->getPosition() ||
                       pos == maze.getLadderPosition();
            }),
        validPositions.end()
    );
    
    if (validPositions.size() < 3) { // Χρειαζόμαστε 1 κλειδί + 2 παγίδες
        return false;
    }
    
    // Τοποθέτηση κλειδιού
    uniform_int_distribution<int> dist(0, static_cast<int>(validPositions.size()) - 1);
    int keyIndex = dist(rng);
    keyPosition = validPositions[keyIndex];
    validPositions.erase(validPositions.begin() + keyIndex);
    
    // Τοποθέτηση 2 παγίδων
    for (int i = 0; i < 2; i++) {
        if (validPositions.empty()) {
            return false;
        }
        
        uniform_int_distribution<int> trapDist(0, static_cast<int>(validPositions.size()) - 1);
        int trapIndex = trapDist(rng);
        trapPositions.push_back(validPositions[trapIndex]);
        validPositions.erase(validPositions.begin() + trapIndex);
    }
    
    return true;
}

Position Game::getRandomValidPosition() {
    vector<Position> validPositions = maze.getValidPositions();
    
    if (validPositions.empty()) {
        return Position(0, 0);
    }
    
    uniform_int_distribution<int> dist(0, static_cast<int>(validPositions.size()) - 1);
    return validPositions[dist(rng)];
}

bool Game::isPositionOccupied(const Position& pos) {
    return pos == grigorakis->getPosition() || 
           pos == asimenia->getPosition() ||
           pos == keyPosition ||
           pos == maze.getLadderPosition() ||
           find(trapPositions.begin(), trapPositions.end(), pos) != trapPositions.end() ||
           find(cagePositions.begin(), cagePositions.end(), pos) != cagePositions.end();
}

void Game::gameLoop() {
    while (!gameWon && !gameLost && currentTurn < 1000) {
        // Καθαρισμός οθόνης
        clear();
        
        // Ενημέρωση παιχνιδιού
        updateGame();
        
        // Σχεδίαση παιχνιδιού
        renderGame();
        
        // Ανανέωση οθόνης
        refresh();
        
        // Καθυστέρηση
        delay(300);
        
        // Έλεγχος για έξοδο (ESC key)
        int ch = getch();
        if (ch == 27) { // ESC
            break;
        }
        
        currentTurn++;
    }
    
    // Τελικό μήνυμα
    if (gameWon) {
        handleWin();
    } else {
        handleLoss();
    }
    
    // Αναμονή για έξοδο
    timeout(-1);
    getch();
}

void Game::updateGame() {
    if (gameWon || gameLost) {
        return;
    }
    
    // ΠΡΟΤΕΡΑΙΟΤΗΤΑ: Ενημέρωση ηρώων για κλουβιά πριν την κίνηση
    updateHeroesAwareness();
    
    // Κίνηση ηρώων (μόνο αν δεν είναι παγιδευμένοι)
    if (!grigorakis->getIsTrapped()) {
        Position newGrigorakisPos = grigorakis->makeMove(maze);
        if (maze.isPassable(newGrigorakisPos) && canMoveToPosition(newGrigorakisPos, grigorakis)) {
            grigorakis->setPosition(newGrigorakisPos);
        }
    }
    
    if (!asimenia->getIsTrapped()) {
        Position newAsimensiaPos = asimenia->makeMove(maze);
        if (maze.isPassable(newAsimensiaPos) && canMoveToPosition(newAsimensiaPos, asimenia)) {
            asimenia->setPosition(newAsimensiaPos);
        }
    }
    
    // Έλεγχος για παγίδες
    checkTraps();
    
    // Έλεγχος για κλειδί
    checkKey();
    
    // Έλεγχος για κλουβιά
    checkCages();
    
    // Έλεγχος συνθηκών νίκης/ήττας
    if (checkWinCondition()) {
        gameWon = true;
    } else if (checkLoseCondition()) {
        gameLost = true;
    }
}

bool Game::checkTraps() {
    bool trapActivated = false;
    
    // Έλεγχος αν κάποιος ήρωας πάτησε παγίδα
    for (int i = 0; i < static_cast<int>(trapPositions.size()); i++) {
        Position trapPos = trapPositions[i];
        
        // Ο Γρηγοράκης μπορεί να παγιδευτεί μόνο αν ΔΕΝ έχει κλειδί
        if (grigorakis->getPosition() == trapPos && !grigorakis->getIsTrapped() && !grigorakis->getHasKey()) {
            grigorakis->setIsTrapped(true);
            cagePositions.push_back(trapPos);
            trapPositions.erase(trapPositions.begin() + i);
            trapActivated = true;
            
            // ΑΜΕΣΗ ενημέρωση της Ασημένιας για το κλουβί
            asimenia->setCagePosition(trapPos);
            break;
        }
        
        // Η Ασημένια μπορεί να παγιδευτεί μόνο αν ΔΕΝ έχει κλειδί
        if (asimenia->getPosition() == trapPos && !asimenia->getIsTrapped() && !asimenia->getHasKey()) {
            asimenia->setIsTrapped(true);
            cagePositions.push_back(trapPos);
            trapPositions.erase(trapPositions.begin() + i);
            trapActivated = true;
            
            // ΑΜΕΣΗ ενημέρωση του Γρηγοράκη για το κλουβί
            grigorakis->setCagePosition(trapPos);
            break;
        }
    }
    
    return trapActivated;
}

bool Game::checkKey() {
    // Έλεγχος αν κάποιος ήρωας πήρε το κλειδί
    if (grigorakis->getPosition() == keyPosition && !grigorakis->getHasKey()) {
        grigorakis->setHasKey(true);
        keyPosition = Position(-1, -1); // Αφαίρεση κλειδιού από χάρτη
        return true;
    }
    
    if (asimenia->getPosition() == keyPosition && !asimenia->getHasKey()) {
        asimenia->setHasKey(true);
        keyPosition = Position(-1, -1); // Αφαίρεση κλειδιού από χάρτη
        return true;
    }
    
    // Ενημέρωση ηρώων για τη θέση του κλειδιού αν το βλέπουν
    vector<Position> grigorakisVisible = grigorakis->getVisiblePositions(maze);
    for (const Position& pos : grigorakisVisible) {
        if (pos == keyPosition && !grigorakis->isKeyFound()) {
            grigorakis->setKeyPosition(keyPosition);
        }
        // Έλεγχος για κλουβιά στο οπτικό πεδίο
        for (const Position& cagePos : cagePositions) {
            if (pos == cagePos && !grigorakis->isCageFound()) {
                grigorakis->setCagePosition(cagePos);
            }
        }
    }
    
    vector<Position> asimensiaVisible = asimenia->getVisiblePositions(maze);
    for (const Position& pos : asimensiaVisible) {
        if (pos == keyPosition && !asimenia->isKeyFound()) {
            asimenia->setKeyPosition(keyPosition);
        }
        // Έλεγχος για κλουβιά στο οπτικό πεδίο
        for (const Position& cagePos : cagePositions) {
            if (pos == cagePos && !asimenia->isCageFound()) {
                asimenia->setCagePosition(cagePos);
            }
        }
    }
    
    return false;
}

bool Game::checkCages() {
    // Έλεγχος αν κάποιος ήρωας με κλειδί ξεκλείδωσε κλουβί
    for (int i = 0; i < static_cast<int>(cagePositions.size()); i++) {
        Position cagePos = cagePositions[i];
        
        // Ο Γρηγοράκης ξεκλειδώνει την Ασημένια
        if (grigorakis->getPosition() == cagePos && grigorakis->getHasKey() && asimenia->getIsTrapped()) {
            asimenia->setIsTrapped(false);
            cagePositions.erase(cagePositions.begin() + i);
            grigorakis->setHasKey(false); // Χάνει το κλειδί
            return true;
        }
        
        // Η Ασημένια ξεκλειδώνει τον Γρηγοράκη
        if (asimenia->getPosition() == cagePos && asimenia->getHasKey() && grigorakis->getIsTrapped()) {
            grigorakis->setIsTrapped(false);
            cagePositions.erase(cagePositions.begin() + i);
            asimenia->setHasKey(false); // Χάνει το κλειδί
            return true;
        }
    }
    
    return false;
}

void Game::updateHeroesAwareness() {
    // Ενημέρωση ηρώων για κλουβιά στην περιοχή τους
    vector<Position> grigorakisVisible = grigorakis->getVisiblePositions(maze);
    vector<Position> asimensiaVisible = asimenia->getVisiblePositions(maze);
    
    // Ο Γρηγοράκης μαθαίνει για κλουβιά
    for (const Position& pos : grigorakisVisible) {
        for (const Position& cagePos : cagePositions) {
            if (pos == cagePos && !grigorakis->isCageFound()) {
                grigorakis->setCagePosition(cagePos);
            }
        }
    }
    
    // Η Ασημένια μαθαίνει για κλουβιά
    for (const Position& pos : asimensiaVisible) {
        for (const Position& cagePos : cagePositions) {
            if (pos == cagePos && !asimenia->isCageFound()) {
                asimenia->setCagePosition(cagePos);
            }
        }
    }
}

bool Game::canMoveToPosition(const Position& pos, Hero* hero) {
    // Έλεγχος αν η θέση είναι κλουβί
    for (const Position& cagePos : cagePositions) {
        if (pos == cagePos) {
            // Αν έχει κλειδί, μπορεί να μπει στο κλουβί για να το ξεκλειδώσει
            if (hero->getHasKey()) {
                return true;
            }
            // Αν δεν έχει κλειδί, δεν μπορεί να μπει
            return false;
        }
    }
    
    return true; // Μπορεί να κινηθεί στη θέση
}

bool Game::checkWinCondition() {
    // Νίκη αν οι ήρωες είναι στην ίδια θέση
    return grigorakis->getPosition() == asimenia->getPosition();
}

bool Game::checkLoseCondition() {
    // Ήττα αν περάσουν 1000 τέρμινα
    if (currentTurn >= 1000) {
        return true;
    }
    
    // Ήττα αν και οι δύο είναι παγιδευμένοι
    if (grigorakis->getIsTrapped() && asimenia->getIsTrapped()) {
        // Αν δεν υπάρχει κλειδί στον χάρτη (κάποιος το έχει πάρει)
        // αλλά κανένας δεν μπορεί να κινηθεί για να σώσει τον άλλον
        if (keyPosition.getX() == -1 && keyPosition.getY() == -1) {
            return true; // Και οι δύο παγιδευμένοι, κανένας δεν έχει κλειδί
        }
        
        // Αν το κλειδί υπάρχει στον χάρτη αλλά κανένας δεν μπορεί να το πάρει
        // (και οι δύο παγιδευμένοι)
        if (keyPosition.getX() != -1 && keyPosition.getY() != -1) {
            return true; // Το κλειδί υπάρχει αλλά κανένας δεν μπορεί να κινηθεί
        }
    }
    
    return false;
}

void Game::handleWin() {
    // Πρώτα εμφάνισε μήνυμα νίκης
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(maze.getHeight() + 3, 0, "*** VICTORY! ***");
    mvprintw(maze.getHeight() + 4, 0, "Grigorakis and Asimenia found each other!");
    mvprintw(maze.getHeight() + 5, 0, "Removing walls...");
    attroff(COLOR_PAIR(COLOR_UI));
    refresh();
    delay(2000);
    
    // Αφαίρεση τοίχων με animation
    if (!wallsRemoved) {
        removeWallsAnimation();
        wallsRemoved = true;
    }
    
    // Κίνηση προς τη σκάλα
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(maze.getHeight() + 5, 0, "Moving to the ladder...");
    attroff(COLOR_PAIR(COLOR_UI));
    refresh();
    
    moveToLadder();
    
    // Τελικό μήνυμα
    clear();
    renderGame();
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(maze.getHeight() + 3, 0, "*** THE KINGDOM IS SAVED! ***");
    mvprintw(maze.getHeight() + 4, 0, "Grigorakis and Asimenia escaped together!");
    mvprintw(maze.getHeight() + 5, 0, "Love conquers all!");
    mvprintw(maze.getHeight() + 6, 0, "Press any key to exit...");
    attroff(COLOR_PAIR(COLOR_UI));
    refresh();
}

void Game::handleLoss() {
    // Μήνυμα ήττας
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(maze.getHeight() + 3, 0, "*** GAME OVER ***");
    
    if (currentTurn >= 1000) {
        mvprintw(maze.getHeight() + 4, 0, "Time ran out! The kingdom is destroyed...");
    } else {
        mvprintw(maze.getHeight() + 4, 0, "The heroes lost all hope...");
    }
    
    mvprintw(maze.getHeight() + 5, 0, "Press any key to exit...");
    attroff(COLOR_PAIR(COLOR_UI));
}

void Game::removeWallsAnimation() {
    // Animation για αφαίρεση τοίχων
    for (int phase = 0; phase < 5; phase++) {
        clear();
        
        // Σχεδίαση λαβύρινθου με σταδιακή αφαίρεση τοίχων
        for (int y = 0; y < maze.getHeight(); y++) {
            for (int x = 0; x < maze.getWidth(); x++) {
                Position pos(x, y);
                char cell = maze.getCell(pos);
                
                // Σταδιακή αφαίρεση τοίχων
                if (cell == '*' && (x + y + phase) % 3 == 0) {
                    cell = ' '; // Αφαίρεση τοίχου
                }
                
                if (cell == '*') {
                    attron(COLOR_PAIR(COLOR_WALL));
                    mvaddch(y, x, '*');
                    attroff(COLOR_PAIR(COLOR_WALL));
                } else {
                    mvaddch(y, x, ' ');
                }
            }
        }
        
        // Σχεδίαση ηρώων
        renderHeroes();
        
        // Σχεδίαση σκάλας
        Position ladderPos = maze.getLadderPosition();
        attron(COLOR_PAIR(COLOR_LADDER));
        mvaddch(ladderPos.getY(), ladderPos.getX(), 'L');
        attroff(COLOR_PAIR(COLOR_LADDER));
        
        // Μήνυμα
        attron(COLOR_PAIR(COLOR_UI));
        mvprintw(maze.getHeight() + 1, 0, "The power of love destroys the walls! Phase %d/5", phase + 1);
        attroff(COLOR_PAIR(COLOR_UI));
        
        refresh();
        delay(800);
    }
    
    // Τελική αφαίρεση όλων των τοίχων
    maze.removeWalls();
    clear();
    renderGame();
    refresh();
    delay(1000);
}

void Game::moveToLadder() {
    Position ladderPos = maze.getLadderPosition();
    
    // Κίνηση προς τη σκάλα (απλοποιημένη)
    while (grigorakis->getPosition() != ladderPos || asimenia->getPosition() != ladderPos) {
        // Κίνηση Γρηγοράκη προς σκάλα
        if (grigorakis->getPosition() != ladderPos) {
            Position currentPos = grigorakis->getPosition();
            int dx = 0, dy = 0;
            
            if (currentPos.getX() < ladderPos.getX()) dx = 1;
            else if (currentPos.getX() > ladderPos.getX()) dx = -1;
            
            if (currentPos.getY() < ladderPos.getY()) dy = 1;
            else if (currentPos.getY() > ladderPos.getY()) dy = -1;
            
            Position newPos(currentPos.getX() + dx, currentPos.getY() + dy);
            grigorakis->setPosition(newPos);
        }
        
        // Κίνηση Ασημένιας προς σκάλα
        if (asimenia->getPosition() != ladderPos) {
            Position currentPos = asimenia->getPosition();
            int dx = 0, dy = 0;
            
            if (currentPos.getX() < ladderPos.getX()) dx = 1;
            else if (currentPos.getX() > ladderPos.getX()) dx = -1;
            
            if (currentPos.getY() < ladderPos.getY()) dy = 1;
            else if (currentPos.getY() > ladderPos.getY()) dy = -1;
            
            Position newPos(currentPos.getX() + dx, currentPos.getY() + dy);
            asimenia->setPosition(newPos);
        }
        
        clear();
        renderGame();
        refresh();
        delay(200);
    }
}

void Game::delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void Game::renderGame() {
    renderMaze();
    renderObjects();
    renderHeroes();
    renderUI();
}

void Game::renderMaze() {
    // Σχεδίαση λαβύρινθου
    for (int y = 0; y < maze.getHeight(); y++) {
        for (int x = 0; x < maze.getWidth(); x++) {
            Position pos(x, y);
            char displayChar = getDisplayChar(pos);
            int colorPair = getColorPair(pos);
            
            if (colorPair > 0) {
                attron(COLOR_PAIR(colorPair));
            }
            
            mvaddch(y, x, displayChar);
            
            if (colorPair > 0) {
                attroff(COLOR_PAIR(colorPair));
            }
        }
    }
}

void Game::renderHeroes() {
    // Σχεδίαση Γρηγοράκη
    Position grigorakisPos = grigorakis->getPosition();
    attron(COLOR_PAIR(COLOR_GRIGORAKIS));
    mvaddch(grigorakisPos.getY(), grigorakisPos.getX(), grigorakis->getSymbol());
    attroff(COLOR_PAIR(COLOR_GRIGORAKIS));
    
    // Σχεδίαση Ασημένιας
    Position asimensiaPos = asimenia->getPosition();
    attron(COLOR_PAIR(COLOR_ASIMENIA));
    mvaddch(asimensiaPos.getY(), asimensiaPos.getX(), asimenia->getSymbol());
    attroff(COLOR_PAIR(COLOR_ASIMENIA));
}

void Game::renderObjects() {
    // Σχεδίαση κλειδιού (αν υπάρχει)
    if (keyPosition.getX() != -1 && keyPosition.getY() != -1) {
        attron(COLOR_PAIR(COLOR_KEY));
        mvaddch(keyPosition.getY(), keyPosition.getX(), 'K');
        attroff(COLOR_PAIR(COLOR_KEY));
    }
    
    // Σχεδίαση παγίδων (αόρατες στους ήρωες, αλλά ορατές στους θεατές)
    for (const Position& trapPos : trapPositions) {
        attron(COLOR_PAIR(COLOR_TRAP));
        mvaddch(trapPos.getY(), trapPos.getX(), 'T');
        attroff(COLOR_PAIR(COLOR_TRAP));
    }
    
    // Σχεδίαση κλουβιών
    for (const Position& cagePos : cagePositions) {
        attron(COLOR_PAIR(COLOR_CAGE));
        mvaddch(cagePos.getY(), cagePos.getX(), 'C');
        attroff(COLOR_PAIR(COLOR_CAGE));
    }
    
    // Σχεδίαση σκάλας
    Position ladderPos = maze.getLadderPosition();
    attron(COLOR_PAIR(COLOR_LADDER));
    mvaddch(ladderPos.getY(), ladderPos.getX(), 'L');
    attroff(COLOR_PAIR(COLOR_LADDER));
}

void Game::renderUI() {
    int uiY = maze.getHeight() + 1;
    
    attron(COLOR_PAIR(COLOR_UI));
    
    // Τέρμινα
    mvprintw(uiY, 0, "Turn: %d/1000", currentTurn);
    
    // Κατάσταση Γρηγοράκη
    mvprintw(uiY + 1, 0, "Grigorakis (G): (%d, %d) %s %s", 
             grigorakis->getPosition().getX(), 
             grigorakis->getPosition().getY(),
             grigorakis->getHasKey() ? "[HAS KEY]" : "",
             grigorakis->getIsTrapped() ? "[TRAPPED]" : "");
    
    // Κατάσταση Ασημένιας
    mvprintw(uiY + 2, 0, "Asimenia (S): (%d, %d) %s %s", 
             asimenia->getPosition().getX(), 
             asimenia->getPosition().getY(),
             asimenia->getHasKey() ? "[HAS KEY]" : "",
             asimenia->getIsTrapped() ? "[TRAPPED]" : "");
    
    // Απόσταση μεταξύ ηρώων
    int distance = grigorakis->getPosition().manhattanDistance(asimenia->getPosition());
    mvprintw(uiY + 3, 0, "Distance between heroes: %d", distance);
    
    // Οδηγίες
    mvprintw(uiY + 5, 0, "Legend: G=Grigorakis, S=Asimenia, K=Key, T=Trap, C=Cage, L=Ladder");
    mvprintw(uiY + 6, 0, "Press ESC to quit");
    
    attroff(COLOR_PAIR(COLOR_UI));
}

char Game::getDisplayChar(const Position& pos) {
    // Προτεραιότητα: Ήρωες > Αντικείμενα > Λαβύρινθος
    
    // Έλεγχος για ήρωες
    if (pos == grigorakis->getPosition()) {
        return grigorakis->getSymbol();
    }
    if (pos == asimenia->getPosition()) {
        return asimenia->getSymbol();
    }
    
    // Έλεγχος για αντικείμενα
    if (pos == keyPosition) {
        return 'K';
    }
    
    for (const Position& trapPos : trapPositions) {
        if (pos == trapPos) {
            return 'T';
        }
    }
    
    for (const Position& cagePos : cagePositions) {
        if (pos == cagePos) {
            return 'C';
        }
    }
    
    if (pos == maze.getLadderPosition()) {
        return 'L';
    }
    
    // Επιστροφή χαρακτήρα λαβύρινθου
    return maze.getCell(pos);
}

int Game::getColorPair(const Position& pos) {
    // Προτεραιότητα: Ήρωες > Αντικείμενα > Λαβύρινθος
    
    // Έλεγχος για ήρωες
    if (pos == grigorakis->getPosition()) {
        return COLOR_GRIGORAKIS;
    }
    if (pos == asimenia->getPosition()) {
        return COLOR_ASIMENIA;
    }
    
    // Έλεγχος για αντικείμενα
    if (pos == keyPosition) {
        return COLOR_KEY;
    }
    
    for (const Position& trapPos : trapPositions) {
        if (pos == trapPos) {
            return COLOR_TRAP;
        }
    }
    
    for (const Position& cagePos : cagePositions) {
        if (pos == cagePos) {
            return COLOR_CAGE;
        }
    }
    
    if (pos == maze.getLadderPosition()) {
        return COLOR_LADDER;
    }
    
    // Χρώμα λαβύρινθου
    if (maze.isWall(pos)) {
        return COLOR_WALL;
    }
    
    return 0; // Προεπιλεγμένο χρώμα
}