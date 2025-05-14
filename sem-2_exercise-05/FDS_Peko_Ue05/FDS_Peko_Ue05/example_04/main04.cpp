// example_04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <iomanip>
#include "pfc-mini.hpp"

namespace fs = std::filesystem;

void list_directory_recursive(const fs::path& path, int depth = 0) {
    try {
        if (!fs::exists(path)) {
            std::cout << "Fehler: Der angegebene Pfad existiert nicht." << std::endl;
            return;
        }
        
        if (!fs::is_directory(path)) {
            std::cout << "Fehler: Der angegebene Pfad ist kein Verzeichnis." << std::endl;
            return;
        }
        
        // Über alle Einträge im Verzeichnis iterieren
        for (const auto& entry : fs::directory_iterator(path)) {
            // Einrückung für die Hierarchie
            std::string indent(depth * 2, ' ');
            
            // Datei- oder Verzeichnisname ausgeben
            std::cout << indent << "|-" << entry.path().filename().string();
            
            // Zusätzliche Informationen für Dateien ausgeben
            if (fs::is_regular_file(entry)) {
                // Dateigröße in Human readable Format ausgeben
                auto file_size = fs::file_size(entry);
                std::cout << " (";

                if (file_size >= 1024) {
                    double size_kb = file_size / 1024.0;
                    if (size_kb >= 1024) {
                        double size_mb = size_kb / 1024.0;
                        std::cout << std::fixed << std::setprecision(2) << size_mb << " MB";
                    } else {
                        std::cout << std::fixed << std::setprecision(2) << size_kb << " KB";
                    }
                }
                else {
                    std::cout << file_size << " Bytes";
                }
                
                std::cout << ")";
            }
            
            std::cout << std::endl;
            
            // Rekursiv für Unterverzeichnisse aufrufen
            if (fs::is_directory(entry)) {
                list_directory_recursive(entry.path(), depth + 1);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Fehler beim Zugriff auf Dateisystem: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Fehler: " << e.what() << std::endl;
    }
}

int main() {
    // Benutzer nach einem Verzeichnispfad fragen
    std::string path;
    std::cout << "Bitte geben Sie den Pfad zu einem Verzeichnis ein: ";
    std::getline(std::cin, path);
    
    if (path.empty()) {
        std::cout << "Kein Pfad angegeben. Verwende aktuelles Verzeichnis." << std::endl;
        path = fs::current_path().string();
    }
    
    std::cout << std::endl << "path = " << path << std::endl << std::endl;
    list_directory_recursive(path);
    
    // Testfälle
    std::cout << std::endl << std::endl << "Testfaelle:" << std::endl;
    
    std::cout << std::endl << "Testfall 1: Aktuelles Verzeichnis" << std::endl;
    std::cout << "current_path = " << fs::current_path().string() << std::endl;
    list_directory_recursive(fs::current_path());
    
    std::cout << std::endl << "Testfall 2: Nicht existierendes Verzeichnis" << std::endl;
    list_directory_recursive("nicht_existierender_ordner");
    
    std::cout << std::endl << "Testfall 3: Datei statt Verzeichnis" << std::endl;
    fs::path self_path = fs::path(__FILE__);
    std::cout << "self_path = " << self_path.string() << std::endl;
    if (fs::exists(self_path)) {
        list_directory_recursive(self_path);
    } else {
        std::cout << "Kann die Programmdatei nicht finden für den Test." << std::endl;
    }
    
    return 0;
}
