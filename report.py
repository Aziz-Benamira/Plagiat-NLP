import os
import time
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.lib.colors import red, magenta, black, gold

def txt_to_pdf(input_file, output_file):
    # Vérifier si le fichier texte existe
    if not os.path.exists(input_file):
        print(f"Erreur : Le fichier {input_file} n'existe pas.")
        return

    # Créer ou écraser le fichier PDF
    c = canvas.Canvas(output_file, pagesize=letter)
    width, height = letter

    # Lire le fichier texte
    with open(input_file, 'r', encoding='utf-8') as file:
        text = file.readlines()

    # Position initiale du texte
    x = 50
    y = height - 50
    line_height = 14  # Hauteur des lignes
    spacing = 20  # Espacement entre les parties

    # Fonction pour écrire du texte avec une couleur spécifique
    def write_text(c, text, color, x, y, font="Helvetica", size=12):
        c.setFont(font, size)
        c.setFillColor(color)
        c.drawString(x, y, text)

    # Parcourir chaque ligne du texte
    for line in text:
        line = line.strip()

        if not line:  # Ajouter un espacement entre les sections vides
            y -= spacing
            continue

        # Reconnaître les titres avec #
        if line.startswith("###"):
            write_text(c, line[3:].strip(), black, x, y, font="Helvetica-Bold", size=14)
        elif line.startswith("##"):
            write_text(c, line[2:].strip(), black, x, y, font="Helvetica-Bold", size=16)
        elif line.startswith("#"):
            write_text(c, line[1:].strip(), black, x, y, font="Helvetica-Bold", size=18)
        else:
            i = 0
            while i < len(line):
                if line.startswith("<red>", i):
                    i += 5
                    end = line.find("</red>", i)
                    word = line[i:end]
                    write_text(c, word, red, x, y)
                    x += c.stringWidth(word, "Helvetica", 12)
                    i = end + 6
                elif line.startswith("<yellow>", i):
                    i += 8
                    end = line.find("</yellow>", i)
                    word = line[i:end]
                    write_text(c, word, gold, x, y)  # Utilisation de "gold" pour une teinte moins claire
                    x += c.stringWidth(word, "Helvetica", 12)
                    i = end + 9
                elif line.startswith("<magenta>", i):
                    i += 9
                    end = line.find("</magenta>", i)
                    word = line[i:end]
                    write_text(c, word, magenta, x, y)
                    x += c.stringWidth(word, "Helvetica", 12)
                    i = end + 10
                else:
                    write_text(c, line[i], black, x, y)
                    x += c.stringWidth(line[i], "Helvetica", 12)
                    i += 1

                # Si la ligne est trop longue, passe à la ligne suivante
                if x > width - 50:
                    x = 50
                    y -= line_height

            # Réinitialiser la position horizontale pour la prochaine ligne
            x = 50
            y -= line_height

        # Ajouter un espacement entre les sections
        if line.startswith("#"):
            y -= spacing

        # Ajouter une nouvelle page si l'espace vertical est insuffisant
        if y < 50:
            c.showPage()
            y = height - 50

    c.save()
    print(f"PDF mis à jour : {output_file}")

def monitor_and_update_pdf(input_file, output_file, check_interval=5, max_attempts=12):
    last_modification_time = None
    attempts = 0

    while attempts < max_attempts:
        if os.path.exists(input_file):
            current_modification_time = os.path.getmtime(input_file)
            if last_modification_time is None or current_modification_time > last_modification_time:
                print(f"Mise à jour détectée dans {input_file}. Génération du PDF...")
                txt_to_pdf(input_file, output_file)
                last_modification_time = current_modification_time
                attempts = 0  # Réinitialiser les tentatives
        else:
            print(f"En attente du fichier {input_file}... ({attempts + 1}/{max_attempts})")
            attempts += 1

        time.sleep(check_interval)

    print(f"Arrêt du programme : Le fichier {input_file} est introuvable après plusieurs tentatives.")

# Exemple d'utilisation
monitor_and_update_pdf('output.txt', 'output.pdf')
