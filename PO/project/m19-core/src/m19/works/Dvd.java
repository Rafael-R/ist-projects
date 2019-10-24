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
    public String toString() {
        return _id.toString() + " - " + _actualCopies.toString() + " de " +
               _numberCopies.toString() + " - DVD - " + _title + " - " + 
               _price.toString() + " - " + _category.toString() + " - " +
               _director + " - " + _IGAC;
    }
    
}