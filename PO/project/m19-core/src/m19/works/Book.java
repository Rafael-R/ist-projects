package m19.works;

public class Book extends Work {
    
    private String _author;
    private String _ISBN;

    public Book(int id, int numberCopies, String title, int price, Category category, String author, String ISBN) {
        super(id, numberCopies, title, price, category);
        _author = author;
        _ISBN = ISBN;
    }

    @Override
    public boolean hasTerm(String term) {
        if (_author.contains(term) || _ISBN.contains(term) ) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return _id + " - " + _actualCopies + " de " + _numberCopies + " - Livro - " +
        _title + " - " + _price + " - " + _category + " - " + _author + " - " + _ISBN;
    }

}