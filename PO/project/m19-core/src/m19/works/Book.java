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

    /**
	 * @param term
	 */
    @Override
    public boolean hasTerm(String term) {
        return super.hasTerm(term) ||
               _author.toLowerCase().contains(term.toLowerCase());
    }

    @Override
    public String toString() {
        return _id + " - " + _availableCopies + " de " + _copies + " - Livro - " +
        _title + " - " + _price + " - " + _category + " - " + _author + " - " + _isbn;
    }

}