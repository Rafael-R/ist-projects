package m19.works;

public class Book extends Work {
    
    private String _author;
    private String _isbn;

    public Book(int id, String title, String author, int price, 
                String category, String isbn, int copies) {
        super(id, title, price, category, copies);
        _author = author;
        _isbn = isbn;
    }

    @Override
    public boolean hasTerm(String term) {
        if (_author.contains(term) || _isbn.contains(term) ) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return _id + " - " + _actualCopies + " de " + _numberCopies + " - Livro - " +
        _title + " - " + _price + " - " + _category + " - " + _author + " - " + _isbn;
    }

}