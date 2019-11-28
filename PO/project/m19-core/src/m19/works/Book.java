package m19.works;

public class Book extends Work {

    private static final long serialVersionUID = 201911092304L;
    
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
    
    public String typeString() {
        return " - Livro - ";
    }

    public String infoString() {
        return _author + " - " + _isbn;
    }

}