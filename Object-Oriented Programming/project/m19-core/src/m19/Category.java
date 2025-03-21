package m19;

public enum Category { 

    FICTION {
        @Override
        public String toString() {
            return "Ficção";
        }
    }, 
    SCITECH {
        @Override
        public String toString() {
            return "Técnica e Científica";
        }
    },
    REFERENCE {
        @Override
        public String toString() {
            return "Referência";
        }
    };

}

    