import wikipediaapi
import os

def fetch_wikipedia_articles(titles, output_folder):
    # Initialize the Wikipedia API
    wiki = wikipediaapi.Wikipedia(
    language='fr',
    user_agent='MyAppName/1.0 (https://github.com/username/repository; email@example.com)'
)

    # Create output folder if it doesn't exist
    os.makedirs(output_folder, exist_ok=True)

    for title in titles:
        # Fetch the Wikipedia page
        page = wiki.page(title)

        # Check if the page exists
        if page.exists():
            # Save the content as a .txt file
            file_path = os.path.join(output_folder, f"{title.replace('/', '_')}.txt")
            with open(file_path, "w", encoding="utf-8") as file:
                file.write(page.text)
            print(f"Saved: {title}")
        else:
            print(f"Page not found: {title}")

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
    output_dir = "Corpus/FR"

    # Fetch articles and save them
    fetch_wikipedia_articles(article_titles, output_dir)
