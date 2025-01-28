import wikipediaapi
import os

# Un code python pour télécharger des articles Wikipedia pour remplir le corpus
def fetch_wikipedia_articles(titles, output_folder):
    # Creer une instance de wikipediaapi
    wiki = wikipediaapi.Wikipedia(
        language='fr',
        user_agent='MyAppName/1.0 (https://github.com/username/repository; email@example.com)',
        extract_format=wikipediaapi.ExtractFormat.WIKI
    )

    # Creer le dossier de sortie s'il n'existe pas
    os.makedirs(output_folder, exist_ok=True)
    def clean_text(text):
        # Remplacer les caractères français par leurs équivalents ASCII
        char_map = {
            'é': 'e', 'è': 'e', 'ê': 'e', 'ë': 'e',
            'à': 'a', 'â': 'a', 'ä': 'a',
            'î': 'i', 'ï': 'i',
            'ô': 'o', 'ö': 'o',
            'ù': 'u', 'û': 'u', 'ü': 'u',
            'ç': 'c',
            'É': 'E', 'È': 'E', 'Ê': 'E', 'Ë': 'E',
            'À': 'A', 'Â': 'A', 'Ä': 'A',
            'Î': 'I', 'Ï': 'I',
            'Ô': 'O', 'Ö': 'O',
            'Ù': 'U', 'Û': 'U', 'Ü': 'U',
            'Ç': 'C'
        }
        for k, v in char_map.items():
            text = text.replace(k, v)
        return text

    for title in titles:
        page = wiki.page(title)

        # Verifier si la page existe
        if page.exists():
            # Nettoyer le texte pour supprimer les caractères spéciaux
            cleaned_text = clean_text(page.text)
            
            # Enregistrer le texte dans un fichier
            file_path = os.path.join(output_folder, f"{clean_text(title).replace('/', '_')}.fr")
            with open(file_path, "w", encoding="ascii", errors='ignore') as file:
                file.write(cleaned_text)
            print(f"Saved: {title}")
        else:
            print(f"Page not found: {title}")

# Rest of the code remains the same...

# Example usage
if __name__ == "__main__":
    # List of Wikipedia article titles
    article_titles = [
        "France",
        "Tunisie",
        "Perceptron multicouche"
    ]

    # Folder to save the articles
    output_dir = "Corpus/"

    # Fetch articles and save them
    fetch_wikipedia_articles(article_titles, output_dir)
