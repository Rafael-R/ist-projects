package m19.works;

public class Dvd extends Work {
    
    private String _director;
    private String _IGAC;

    public Dvd(int id, int numberCopies, String title, int price, Category category, String director, String IGAC) {
        super(id, numberCopies, title, price, category);
        _director = director;
        _IGAC = IGAC;
    }

    @Override
    public boolean hasTerm(String term) {
        if (_director.contains(term) || _IGAC.contains(term) ) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return _id + " - " + _actualCopies + " de " + _numberCopies + " - DVD - " +
        _title + " - " + _price + " - " + _category + " - " + _director + " - " + _IGAC;
    }
    
}