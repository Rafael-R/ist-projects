package m19;

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
    public boolean infoHasTerm(String term) {
        return _author.toLowerCase().contains(term.toLowerCase());
    }
    
    public String typeString() {
        return " - Livro - ";
    }

    public String infoString() {
        return " - " + _author + " - " + _isbn;
    }

}