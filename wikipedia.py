import wikipediaapi
import os

def fetch_wikipedia_articles(titles, output_folder):
    # Initialize the Wikipedia API with UTF-8 encoding
    wiki = wikipediaapi.Wikipedia(
        language='fr',
        user_agent='MyAppName/1.0 (https://github.com/username/repository; email@example.com)',
        extract_format=wikipediaapi.ExtractFormat.WIKI
    )

    # Create output folder if it doesn't exist
    os.makedirs(output_folder, exist_ok=True)
    def clean_text(text):
        # Map common French characters to ASCII equivalents
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
        # Fetch the Wikipedia page
        page = wiki.page(title)

        # Check if the page exists
        if page.exists():
            # Clean text by replacing French characters with ASCII equivalents
            cleaned_text = clean_text(page.text)
            
            # Save the content as a .txt file with ASCII encoding
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
        "Intelligence artificielle",
        "Traitement automatique des langues",
        "Apprentissage automatique",
        "Apprentissage profond",
        "Science des données"
    ]

    # Folder to save the articles
    output_dir = "Corpus/"

    # Fetch articles and save them
    fetch_wikipedia_articles(article_titles, output_dir)
