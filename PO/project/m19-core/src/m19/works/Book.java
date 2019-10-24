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
    public String toString() {
        return _id.toString() + " - " + _actualCopies.toString() + " de " +
               _numberCopies.toString() + " - Livro - " + _title + " - " + 
               _price.toString() + " - " + _category.toString() + " - " +
               _author + " - " + _ISBN;
    }

}