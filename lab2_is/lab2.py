import os
import spacy
from collections import Counter

# загрузка языковой модели для русского языка
nlp = spacy.load("en_core_web_sm")

# получение стоп-слов из языковой модели
stop_words = set(nlp.Defaults.stop_words)

# путь к папке с текстовыми документами
collection_path = 'collection'

# лексический анализ, лемматизация и удаление стоп-слов
def preprocess_text(text):
    doc = nlp(text)     # разделение текста на термы и присвоение каждому стемм
    # токен – хоть какой элемент (терм – слово/словосочетание), лемма ~ стемм; только буквенные и не стоп-слова
    tokens = [token.lemma_ for token in doc if token.is_alpha and token.lemma_.lower() not in stop_words]
    return tokens

# расчет весовых коэффициентов для термов
def calculate_weights(documents):
    weights = {}
    for doc_name, doc_content in documents.items():
        tokens = preprocess_text(doc_content)
        term_freq = Counter(tokens) 
        weights[doc_name] = term_freq
    return weights

# расчет релевантности
def calculate_relevance(query, weights):
    query_tokens = preprocess_text(query)
    query_freq = Counter(query_tokens)  # ключи – термы из запроса, а значения - их частота встречаемости в запросе

    relevance_scores = {}
    for doc_name, term_freq in weights.items():
        # нахождение общих термов между запросом и текущим документом
        common_terms = set(query_freq.keys()) & set(term_freq.keys())
        # умножение частоты терма в запросе на частоту в текущем документе, затем суммирование
        score = sum(query_freq[term] * term_freq[term] for term in common_terms)
        relevance_scores[doc_name] = score

    return relevance_scores

# вывод результатов
def display_results(sorted_results):
    for doc_name, similarity in sorted_results:
        common_terms = set(weights[doc_name].keys()) & set(preprocess_text(user_query))
        print(f"Document: {doc_name}, Similar Terms: {', '.join(common_terms)}, Relevance: {similarity}")

# считывание документов
documents = {}
for file_name in os.listdir(collection_path):
    if file_name.endswith('.txt'):
        with open(os.path.join(collection_path, file_name), 'r', encoding='utf-8') as file:
            documents[file_name] = file.read()

# расчет весовых коэффициентов
weights = calculate_weights(documents)

user_query = input("введите запрос: ")

# расчет релевантности
relevance_scores = calculate_relevance(user_query, weights)

# сортировка результатов по релевантности по убыванию (словарь doc_name: score, сортировка по второму элементу)
sorted_results = sorted(relevance_scores.items(), key=lambda x: x[1], reverse=True)

display_results(sorted_results)
