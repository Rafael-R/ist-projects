package m19;

public class Dvd extends Work {
    
    private String _director;
    private String _igac;

    public Dvd(int id, String title, String director, int price, 
            String category, String igac, int copies) {
        super(id, title, price, category, copies);
        _director = director;
        _igac = igac;
    }

    /**
	 * @param term
	 */
    @Override
    public boolean hasTerm(String term) {
        return super.hasTerm(term) || 
               _director.toLowerCase().contains(term.toLowerCase());
    }

    public String typeString() {
        return " - DVD - ";
    }

    public String infoString() {
        return _director + " - " + _igac;
    }
    
}