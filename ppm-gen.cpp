#include <iostream> // baza
#include <cstdlib> // rand, exit
#include <ctime> // czas
#include <fstream> // pliki zewnetrzne
#include <string> // getline & to_string
#include <cctype> //isdigit
#include <limits> //fixCIN() (cin.ignore)
#include <vector> // case 3; logika zachowywania dzialania \n

using namespace std;

/*
    (C) Jakub Namyslak, 01.01.2024
    All rights reserved

    Więcej na:
    - https://shatterwares.com
    - https://github.com/jakubekgranie
*/

void fixCIN() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
string toBinary(int dataChar) {
    string binaries;
    while (dataChar != 0) {
        binaries += to_string(dataChar % 2);
        dataChar = dataChar / 2;
    }
    for (size_t i = binaries.length(); i < 7; i++) //size_t
        binaries += '0'; //ZEROFILL
    for (size_t i = 0; i < binaries.length() / 2; i++) {
        int buffer = binaries[i];
        binaries[i] = binaries[binaries.length() - 1 - i];
        binaries[binaries.length() - 1 - i] = buffer;
    }
    return binaries;
}
string countTime(clock_t start, clock_t end) {
    int time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    return to_string(time) + "ms";
}
string testStream(ios_base::openmode mode, string pre, string error, string extension = ".ppm") {
    while (1) {
        fstream file;
        string name;
        cout << pre;
        getline(cin >> ws, name);
        name = name + extension;
        file.open(name, mode);
        if (!file.good()) {
            file.close();
            cout << error;
        }
        else {
            file.close();
            return(name);
        }
    }
}
bool protectedChoiceLoop(string pre, string errPrefix) {
    string tmp;
    do {
        cout << pre;
        cin >> tmp;
        if (cin.fail()) {
            tmp = "x";
            fixCIN();
        }
        if (islower(tmp[0]))
            tmp[0] -= 32;
        if (tmp != "Y" && tmp != "N")
            cout << errPrefix << " Nieprawidlowe dane wejsciowe. Sprobuj ponownie.\n";
    } while (tmp != "Y" && tmp != "N");
    if (tmp == "Y")
        return true;
    else
        return false;
}
int protectedMeasurementLoop(string pre, string errPrefix) {
    int measurement = -1;
    while (measurement < 0 || measurement > 256) {
        cout << pre;
        cin >> measurement;
        if (cin.fail()) {
            measurement = -1;
            fixCIN();
        }
        if (measurement < 0 || measurement > 256)
            cout << errPrefix << " Nieprawidlowe dane wejsciowe. Sprobuj ponownie.\n";
    }
    return measurement;
}
string testInputCompat(int max, bool& reset) {
    cout << "\n[STR] Podaj dane. Stosuj '\\n' jako delimiter linii.\n[STR]> ";
    string input;
    getline(cin >> ws, input);
    if (input.length() * 7 > max && !protectedChoiceLoop("\n[GEN] UWAGA: wykryto dane o wielkosci zbyt duzej jak na podany nosnik. W wyniku konynuowania operacji wystapic moga komplikacje zwiazane z utrata zapisu. Kontynuowac? [Y/N]\n[GEN]> ", "[GEN]")) {
        reset = true;
        cout << "\n";
    }
    return input;
}
void typeErrorCheck(string raw) {
    try {
        int test = stoi(raw);
    }
    catch (const invalid_argument&) {
        cerr << "\n[C_ERR][1] Plik uszkodzony: wykryto niewlasciwe dane. Skonsultuj sie z autorem.\n";
        exit(1);
    }
    catch (...) {
        cerr << "\n[C_ERR][2] Napotkano na niespodziewany problem.\n";
        exit(2);
    }
}
string additionalEncryptionGen(string data) {
    int appearances[95] = { 0 };
    string baseKey, encKey, name;
    fstream file;
    for (int i = 32; i < 127; i++) {
        char charCode;
        do {
            charCode = rand() % 95 + 32;
        } while (charCode == appearances[charCode - 32]);
        appearances[charCode - 32] = charCode;
        baseKey += charCode;
        int appearances2[95] = { 0 };
        for (int j = 32; j < 127; j++) {
            char charCode;
            do {
                charCode = rand() % 95 + 32;
            } while (charCode == appearances2[charCode - 32]);
            appearances2[charCode - 32] = charCode;
            baseKey += charCode;
        }
    }
    /*cout << "\n----------\n";
    for (size_t i = 0; i < baseKey.length(); i += 96) {
        cout << baseKey[i] << " ";
        for (int j = 0; j < 95; j++) {
            cout << baseKey[i + j + 1];
        }
        cout << "\n";
    }*/
    for (int i = 0; i < data.length(); i++)
        encKey += rand() % 95 + 32;
    for (size_t i = 0; i < encKey.length(); i++)
        for (size_t j = 0; j < baseKey.length(); j += 96) {
            bool wasEncoded = false;
            if (encKey[i] == baseKey[j])
                for (size_t k = 0; k < 95; k++)
                    if (k + 32 == data[i]) {
                        data[i] = baseKey[j + k + 1];
                        wasEncoded = true;
                        break;
                    }
            if (wasEncoded)
                break;
        }
    // cout << "\n== DATA =======\nProduct: " << data << "\nBase key (without the '|'): | " << baseKey << " |\nEncryption key: " << encKey << "\n==============\n";
    name = testStream(ios::out, "\n[ENC] Podaj nazwe pliku-klucza.\n[ENC]> ", "[ENC] Blad dostepu do pliku. Sprobuj ponownie.\n", ".key");
    file.open(name, ios::out);
    file << baseKey << "\n" << encKey << "\n";
    file.close();
    return data;
}
int main() {
    srand(time(NULL));
    clock_t start = clock();
    string comms[2] = { "\n[NAME] Podaj nazwe pliku:\n[NAME]> ", "[NAME] Blad dostepu do pliku. Sprobuj ponownie.\n" }, tmp;
    fstream file;
    bool reset = false;
    int opType = 0;
    cout << "---------------------------------";
    while (opType < 1 || opType > 3) {
        cout << "\nWybierz tryb operacji:\n1) Generowanie pliku .ppm\n   Produkt: plik '.ppm'\n2) Wzbogacenie pliku .ppm.\n   Produkt: zawarcie w pliku niezauwazalnych zmian zawierajacych informacje.\n3) Odczytanie pliku .ppm.\n   Produkt: proba odczytania z pliku niezauwazalnych zmian zawierajacych informacje.\n\n[OP]> ";
        cin >> opType;
        if (cin.fail()) {
            opType = 0;
            cout << "[OP] Nieprawidlowe dane wejsciowe.\n";
            fixCIN();
        }
        if (opType < 1 || opType > 3)
            cout << "[OP] Nieprawidlowe dane wejsciowe. Sprobuj ponownie.\n";
    }
    switch (opType) {
    case 1:
        tmp = testStream(ios::out, comms[0], comms[1]);
        file.open(tmp, ios::out);
        break;
    case 2:
        tmp = testStream(ios::in, comms[0], comms[1]);
        file.open(tmp, ios::in);
        break;
    case 3:
        tmp = testStream(ios::in, comms[0], comms[1]);
        file.open(tmp, ios::in);
        break;
    }
    if (file.good()) {
        clock_t startGen, endGen;
        bool genFailed = false;
        switch (opType) {
        case 1:
        {
            string data = "", binaries;
            int y = protectedMeasurementLoop("\nPodaj wysokosc (h e <0, 255>) :\n[H]> ", "[H]>"), x = protectedMeasurementLoop("\nPodaj szerokosc (h e <0, 255>) :\n[H]> ", "[W]>"), b, mode;
            b = mode = 0;
            while (mode < 1 || mode > 5) {
                cout << "\nWybierz tryb:\n1) Standardowy (B = 0)\n2) Pollosowy (B e <0, 255>)\n3) Losowy (R && G && B e <0, 255>)\n4) Wlasny (B jest wybierane manualnie)\n5) Zapis (Napis konwertowany jest na liczbe binarna i zapisywany w b)\n\n[TRYB]> ";
                cin >> mode;
                if (cin.fail()) {
                    fixCIN();
                    mode = 0;
                }
                if (mode < 1 || mode > 5)
                    cout << "[TRYB] Nieprawidlowe dane wejsciowe. Sprobuj ponownie.\n";
            }
            if (mode == 4) {
                b = protectedMeasurementLoop("\nPodaj B (e <0, 255>).\n[B]> ", "[B]");
                mode = 1;
            }
            if (mode == 5) {
                data = testInputCompat(x * y, reset);
                if (reset) {
                    main();
                    return 0;
                }
                if (protectedChoiceLoop("\n[ENC] Czy wlaczyc tryb dwukrotnego szyfrowania? [Y/N]\n[ENC]> ", "[ENC]> "))
                    data = additionalEncryptionGen(data);
            }
            cout << "\n[GEN] Trwa generowanie pliku .ppm. Prosze czekac.\n";
            startGen = clock();
            file << "P3\n" << y << " " << x << "\n255\n";
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    if (mode == 5 && (data.length() > 0 || binaries.length() > 0)) {
                        if (binaries.length() == 0) {
                            binaries = toBinary(data[0]);
                            data.erase(0, 1);
                        }
                        file << i << " " << j << " " << binaries[0] << "  ";
                        binaries.erase(0, 1);
                        if (data.length() == 0 && binaries.length() == 0)
                            continue;
                    }
                    if (mode == 2)
                        b = rand() % 256;
                    if (mode == 1 || mode == 2 || (mode == 5 && data.length() == 0 && binaries.length() == 0))
                        file << i << " " << j << " " << b << "  ";
                    if (mode == 3)
                        file << rand() % 256 << " " << rand() % 256 << " " << rand() % 256 << "  ";
                }
                file << "\n";
            }
            file.close();
            endGen = clock();
            cout << "[GEN] Plik wygenerowany pomyslnie.";
            cout << "\n\n[T] PELNY CZAS: " << countTime(start, clock()) << "\n[T] CZAS GENEROWANIA PLIKU: ";
            if (!genFailed)
                cout << countTime(startGen, endGen);
            else cout << "FAILED";
            break;
        }
        case 2:
        {
            int linesRemaining = 3;
            string data, contents, binaries, final;
            startGen = clock();
            while (getline(file, contents)) {
                size_t len = contents.length(); // -1, size_t
                if (linesRemaining == 0 && len > 4) { // 5 to minimum (X X X)
                    size_t charIndex = 0; //size_t - nie testowano
                    while (charIndex < len) {
                        string raw;
                        int rawInt;
                        for (int i = 0; i < 2; i++) {
                            while (charIndex < len && contents[charIndex] == ' ') charIndex++;
                            while (charIndex < len && contents[charIndex] != ' ') charIndex++;
                        }
                        while (charIndex < len && contents[charIndex] == ' ') charIndex++;
                        for (int i = 0; i < 3; i++) {
                            if (charIndex < len && isdigit(contents[charIndex])) {
                                raw = raw + contents[charIndex];
                                charIndex++;
                            }
                            else break;
                        }
                        if (binaries.length() == 0 && data.length() > 0) {
                            binaries = toBinary(data[0]);
                            data.erase(0, 1);
                        }
                        if (binaries.length() == 0)
                            binaries = "0000000";
                        if (raw.length() > 0) {
                            typeErrorCheck(raw);
                            rawInt = stoi(raw);
                            char rawLast;
                            bool hasBeenChanged = false;
                            if (binaries[0] == '0' && rawInt % 2 != 0) {
                                rawLast = raw[raw.length() - 1] - 1;
                                hasBeenChanged = true;
                            }
                            if (binaries[0] == '1' && rawInt % 2 == 0) {
                                rawLast = raw[raw.length() - 1] + 1;
                                hasBeenChanged = true;
                            }
                            if (hasBeenChanged)
                                contents[charIndex - 1] = rawLast;
                            binaries.erase(0, 1);
                        }
                    }
                }
                if (linesRemaining == 2) {
                    int measurements[2] = { 0 };
                    size_t charIndex = 0;
                    for (int i = 0; i < 2; i++) {
                        string raw;
                        while (charIndex < len && contents[charIndex] == ' ') charIndex++;
                        while (charIndex < len && isdigit(contents[charIndex])) {
                            raw = raw + contents[charIndex];
                            charIndex++;
                        }
                        typeErrorCheck(raw);
                        measurements[i] = stoi(raw);
                    }
                    data = testInputCompat(measurements[0] * measurements[1], reset);
                    if (reset) {
                        main();
                        return 0;
                    }
                    if (protectedChoiceLoop("\n[ENC] Czy wlaczyc tryb dwukrotnego szyfrowania? [Y/N]\n[ENC]> ", "[ENC]> "))
                        data = additionalEncryptionGen(data);
                }
                if (linesRemaining > 0)
                    linesRemaining--;
                final = final + contents + "\n";
            }
            file.close();
            bool state, success;
            state = success = false;
            if (linesRemaining == 0)
                success = true;
            for (int i = 0; i < 3; i++) {
                file.open(tmp, ios::out);
                if (file.good()) {
                    if (success) {
                        state = true;
                        cout << "\n[OVWRITE] Dane zostaly zawarte pomyslnie.";
                        file << final;
                        file.close();
                        break;
                    }
                    else {
                        cout << "\n[OVWRITE] Dane nie zostaly zapisane: totalny brak pamieci.";
                        break;
                    }
                }
                else {
                    file.close();
                    cout << "\n[OVWRITE] Nieudana proba otwarcia pliku. Ponawianie... (proba " << i + 1 << "/3)\n";
                }
            }
            if (!state)
                cout << "\n[OVWRITE] Dane nie zostaly zawarte pomyslnie. Upewnij sie, ze program ma uprawnienia do modyfikacji plikow i sprobuj ponownie.";
            cout << "\n[T] PELNY CZAS: " << countTime(start, clock());
            break;
        }
        case 3:
            startGen = clock();
            int linesRemaining = 3;
            string contents, binaries, final;
            while (getline(file, contents)) {
                if (linesRemaining == 0 && contents.length() > 4) {
                    size_t charIndex = 0, len = contents.length() - 1;
                    while (charIndex < len) {
                        string raw;
                        for (int i = 0; i < 2; i++) {
                            while (charIndex < len && contents[charIndex] == ' ') charIndex++;
                            while (charIndex < len && contents[charIndex] != ' ') charIndex++;
                        }
                        while (charIndex < len && contents[charIndex] == ' ') charIndex++;
                        for (int i = 0; i < 3; i++) {
                            if (charIndex < len && isdigit(contents[charIndex])) {
                                raw = raw + contents[charIndex];
                                charIndex++;
                            }
                            else break;
                        }
                        if (raw.length() > 0) {
                            typeErrorCheck(raw);
                            if (stoi(raw) % 2 == 0)
                                binaries = binaries + "0";
                            else
                                binaries = binaries + "1";
                            if (binaries.length() == 7) {
                                char num = 0;
                                for (int i = 6; i > -1; i--)
                                    num = num + ((binaries[i] - '0') << (6 - i));
                                binaries = "";
                                if (num > 31) final = final + num;
                            }
                        }
                    }
                }
                if (linesRemaining > 0)
                    linesRemaining--;
            }
            file.close();
            if (protectedChoiceLoop("\n[ENC] Czy plik jest zaszyfrowany? [Y/N]\n[ENC]> ", "[ENC]> ")) {
                string baseKey, encKey, data;
                tmp = testStream(ios::in, "\n[ENC] Podaj nazwe pliku-klucza.\n[ENC]> ", "[ENC] Blad dostepu do pliku. Sprobuj ponownie.\n", ".key");
                file.open(tmp, ios::in);
                int line = 0;
                while (getline(file, data)) {
                    if (line == 0)
                        baseKey = data;
                    if (line == 1)
                        encKey = data;
                    line++;
                }
                file.close();
                if (baseKey.length() == 9120 && encKey.length() == final.length()) {
                    for (size_t i = 0; i < final.length(); i++)
                        for (size_t j = 0; j < baseKey.length(); j += 96) {
                            bool wasDecoded = false;
                            if (encKey[i] == baseKey[j])
                                for (int k = 0; k < 95; k++)
                                    if (final[i] == baseKey[j + k + 1]) {
                                        final[i] = k + 32;
                                        wasDecoded = true;
                                        break;
                                    }
                            if (wasDecoded)
                                break;
                        }
                }
                else cout << "[ENC] Uszkodzony plik-klucz.\n";
            }
            vector<string> parsedFinal;
            parsedFinal.push_back("");
            size_t i = 0;
            if (final.length() > 1 && final[0] == '\\' && final[1] == 'n') { // mniejsza ilość porównań niż gdyby siedziało w loopie (3 warunki*1 vs 1 warunek*dlugosc)
                parsedFinal.push_back("");
                i += 2;
            }
            for (; i < final.length(); i++) {
                parsedFinal[parsedFinal.size() - 1] += final[i];
                if (final[i + 1] == '\\' && final[i + 2] == 'n') { //size_t
                    parsedFinal.push_back("");
                    i += 2;
                }
            }
            endGen = clock();
            cout << "\nDane ========\n\n";
            cout << parsedFinal[0];
            for (int i = 1; i < parsedFinal.size(); i++)
                cout << "\n" << parsedFinal[i];
            cout << "\n\n=============\n";
            if (protectedChoiceLoop("\n[READ-SAVE] Czy zapisac dane wyjsciowe w pliku zewnetrznym? [y/n]\n[READ]> ", "[READ-SAVE]")) {
                tmp = testStream(ios::out, "\n[READ-SAVE] Podaj nazwe pliku.\n[READ]> ", "[READ-SAVE] Nieudana proba otwarcia pliku. Sprobuj ponownie.\n", ".txt");
                file.open(tmp, ios::out);
                for (int i = 0; i < parsedFinal.size(); i++)
                    file << parsedFinal[i] << "\n";
                file << "-----\n\n========================================\nDecrypted by PPM - GEN\nhttps:\\\\github.com\\jakubekgranie\\PPM - GEN\n(C) Jakub Namyslak\nAll rights reserved\n========================================";
                file.close();
                cout << "[READ-SAVE] Dane zapisane pomyslnie.\n";
            }
            cout << "\nPELNY CZAS: " << countTime(start, clock()) << "\nCZAS ODCZYTYWANIA: " << countTime(startGen, endGen);
            break;
        }
    }
    cout << "\n---------------------------------\n\n[MAIN] Wpisz [R] by zrestartowac lub cokolwiek innego by zamknac...\n[MAIN]> ";
    cin >> tmp;
    if (tmp == "[R]" || tmp == "[r]") {
        cout << "[MAIN] Rozpoczynanie kolejnej iteracji...\n\n";
        main();
    }
    return 0;
}