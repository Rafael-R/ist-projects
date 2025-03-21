-- Cleanup
DROP TABLE category CASCADE;
DROP TABLE simple_category CASCADE;
DROP TABLE super_category CASCADE;
DROP TABLE has_other CASCADE;
DROP TABLE product CASCADE;
DROP TABLE has_category CASCADE;
DROP TABLE ivm CASCADE;
DROP TABLE retail_point CASCADE;
DROP TABLE installed_at CASCADE;
DROP TABLE shelf CASCADE;
DROP TABLE planogram CASCADE;
DROP TABLE retailer CASCADE;
DROP TABLE responsible_for CASCADE;
DROP TABLE replenishment_event CASCADE;

-- Create tables
CREATE TABLE category
(
    category_name varchar(80),

    PRIMARY KEY (category_name)
);

CREATE TABLE simple_category
(
    category_name varchar(80),

    PRIMARY KEY (category_name),
    FOREIGN KEY (category_name) REFERENCES category (category_name)
);

CREATE TRIGGER insert_category
    BEFORE UPDATE OR INSERT
    ON simple_category
    FOR EACH ROW
EXECUTE PROCEDURE insert_category();


CREATE TABLE super_category
(
    category_name varchar(80),

    PRIMARY KEY (category_name),
    FOREIGN KEY (category_name) REFERENCES category (category_name)
);

CREATE TRIGGER insert_category
    BEFORE UPDATE OR INSERT
    ON super_category
    FOR EACH ROW
EXECUTE PROCEDURE insert_category();


CREATE TABLE has_other
(
    super_category_name varchar(80) NOT NULL,
    category_name       varchar(80),

    PRIMARY KEY (category_name),
    FOREIGN KEY (super_category_name) REFERENCES super_category (category_name),
    FOREIGN KEY (category_name) REFERENCES category (category_name),

    CHECK (ri1(super_category_name, category_name))
);

CREATE TABLE product
(
    ean           numeric(13),
    category_name varchar(80)  NOT NULL,
    description   varchar(255) NOT NULL,

    PRIMARY KEY (ean),
    FOREIGN KEY (category_name) REFERENCES category (category_name),

    -- ean is 13 digit
    CHECK (ean >= 1000000000000 AND ean <= 9999999999999)
);

CREATE TRIGGER insert_product
    AFTER UPDATE OR INSERT
    ON product
    FOR EACH ROW
EXECUTE PROCEDURE insert_product();


CREATE TABLE has_category
(
    ean           numeric(13) NOT NULL,
    category_name varchar(80) NOT NULL,

    FOREIGN KEY (ean) REFERENCES product (ean),
    FOREIGN KEY (category_name) REFERENCES category (category_name),

    CHECK (ean >= 1000000000000 AND ean <= 9999999999999)
);

CREATE TABLE ivm
(
    serial_number numeric(15), -- Arbitrary size
    manufacturer  varchar(80),

    PRIMARY KEY (serial_number, manufacturer)
);

CREATE TABLE retail_point
(
    name     varchar(80),
    district varchar(80) NOT NULL,
    county   varchar(80) NOT NULL,

    PRIMARY KEY (name)
);

CREATE TABLE installed_at
(
    serial_number numeric(15),
    manufacturer  varchar(80),
    location      varchar(80) NOT NULL,

    PRIMARY KEY (serial_number, manufacturer),
    FOREIGN KEY (serial_number, manufacturer) REFERENCES ivm (serial_number, manufacturer),
    FOREIGN KEY (location) REFERENCES retail_point (name)
);

CREATE TABLE shelf
(
    number        numeric(3),
    serial_number numeric(15),
    manufacturer  varchar(80),
    height        numeric(2)  NOT NULL,
    category_name varchar(80) NOT NULL,

    PRIMARY KEY (number, serial_number, manufacturer),
    FOREIGN KEY (serial_number, manufacturer) REFERENCES ivm (serial_number, manufacturer),
    FOREIGN KEY (category_name) REFERENCES category (category_name)
);

CREATE TABLE planogram
(
    ean           numeric(13),
    number        numeric(3),
    serial_number numeric(15),
    manufacturer  varchar(80),
    faces         numeric(2) NOT NULL,
    units         numeric(3) NOT NULL,
    loc           varchar(255),

    PRIMARY KEY (ean, number, serial_number, manufacturer),
    FOREIGN KEY (ean) REFERENCES product (ean),
    FOREIGN KEY (number, serial_number, manufacturer) REFERENCES shelf (number, serial_number, manufacturer),

    CHECK (ean >= 1000000000000 AND ean <= 9999999999999)
);

CREATE TABLE retailer
(
    tin  numeric(15),
    name varchar(80) NOT NULL UNIQUE,

    PRIMARY KEY (tin)
);

CREATE TABLE responsible_for
(
    category_name varchar(80) NOT NULL,
    tin           numeric(15) NOT NULL,
    serial_number numeric(15),
    manufacturer  varchar(80),

    PRIMARY KEY (serial_number, manufacturer),
    FOREIGN KEY (serial_number, manufacturer) REFERENCES ivm (serial_number, manufacturer),
    FOREIGN KEY (tin) REFERENCES retailer (tin),
    FOREIGN KEY (category_name) REFERENCES category (category_name)
);

CREATE TABLE replenishment_event
(
    ean           numeric(13),
    number        numeric(3),
    serial_number numeric(15),
    manufacturer  varchar(80),
    instant       timestamp,
    units         numeric(3)  NOT NULL,
    tin           numeric(15) NOT NULL,

    PRIMARY KEY (ean, number, serial_number, manufacturer, instant),
    FOREIGN KEY (ean, number, serial_number, manufacturer)
        REFERENCES planogram (ean, number, serial_number, manufacturer),
    FOREIGN KEY (tin) REFERENCES retailer (tin),

    CHECK (ean >= 1000000000000 AND ean <= 9999999999999),
    CHECK (ri2(ean, number, serial_number, manufacturer, units)),
    CHECK (ri3(ean, number, serial_number))
);

-- Populate Tables
-- super_category
INSERT INTO super_category
VALUES ('Alimentos'),
       ('Bebidas');

-- simple_category
INSERT INTO simple_category
VALUES ('Fruta'),
       ('Snack'),
       ('Sumo'),
       ('Refrigerante');

-- has_other
INSERT INTO has_other
VALUES ('Alimentos', 'Fruta'),
       ('Alimentos', 'Snack'),
       ('Bebidas', 'Sumo'),
       ('Bebidas', 'Refrigerante');

-- product
INSERT INTO product
VALUES ('1273567894358', 'Sumo', 'Laranja'),
       ('7584367489702', 'Refrigerante', 'Coca-Cola'),
       ('1000245245289', 'Fruta', 'Banana'),
       ('3700000736548', 'Snack', 'Barra Energetica');

-- has_category
INSERT INTO has_category
VALUES ('1000245245289', 'Fruta'),
       ('1000245245289', 'Alimentos'),
       ('3700000736548', 'Snack'),
       ('1273567894358', 'Sumo'),
       ('7584367489702', 'Refrigerante');

-- ivm
INSERT INTO ivm
VALUES (101, 'AutoVending'),
       (201, 'iVM Ltd.'),
       (301, 'Apple'),
       (401, 'WeAutomate'),
       (501, 'IVM Plus'),
       (601, 'IVMs de Qualidade'),
       (701, 'iVendors Ltd.'),
       (702, 'iVendors Ltd.'),
       (703, 'iVendors Ltd.'),
       (704, 'iVendors Ltd.');

-- retail_point
INSERT INTO retail_point
VALUES ('Universidade', 'Porto', 'Porto'),
       ('Escola Secundaria', 'Lisboa', 'Lisboa'),
       ('Pingo Snack', 'Lisboa', 'Loures'),
       ('GrabGo', 'Lisboa', 'Amadora'),
       ('CEPSA', 'Lisboa', 'Amadora'),
       ('GALP', 'Lisboa', 'Odivelas'),
       ('Repsol', 'Porto', 'V.N. Gaia');

-- installed_at
INSERT INTO installed_at
VALUES (101, 'AutoVending', 'Pingo Snack'),
       (201, 'iVM Ltd.', 'GrabGo'),
       (301, 'Apple', 'GALP'),
       (401, 'WeAutomate', 'Repsol'),
       (501, 'IVM Plus', 'Escola Secundaria'),
       (601, 'IVMs de Qualidade', 'CEPSA'),
       (701, 'iVendors Ltd.', 'Universidade'),
       (702, 'iVendors Ltd.', 'Universidade'),
       (703, 'iVendors Ltd.', 'Universidade'),
       (704, 'iVendors Ltd.', 'Universidade');

-- shelf
INSERT INTO shelf
VALUES (10, 101, 'AutoVending', 0.20, 'Fruta'),
       (11, 101, 'AutoVending', 0.25, 'Alimentos'),
       (12, 101, 'AutoVending', 0.25, 'Sumo'),
       (10, 201, 'iVM Ltd.', 0.30, 'Refrigerante'),
       (11, 201, 'iVM Ltd.', 0.30, 'Refrigerante'),
       (12, 201, 'iVM Ltd.', 0.25, 'Alimentos'),
       (10, 301, 'Apple', 0.30, 'Bebidas'),
       (11, 301, 'Apple', 0.30, 'Fruta'),
       (10, 401, 'WeAutomate', 0.35, 'Refrigerante'),
       (11, 401, 'WeAutomate', 0.35, 'Refrigerante'),
       (12, 401, 'WeAutomate', 0.25, 'Sumo'),
       (10, 501, 'IVM Plus', 0.25, 'Snack'),
       (11, 501, 'IVM Plus', 0.25, 'Fruta'),
       (10, 601, 'IVMs de Qualidade', 0.30, 'Alimentos'),
       (11, 601, 'IVMs de Qualidade', 0.30, 'Bebidas'),
       (10, 701, 'iVendors Ltd.', 0.20, 'Alimentos'),
       (11, 701, 'iVendors Ltd.', 0.35, 'Bebidas'),
       (12, 701, 'iVendors Ltd.', 0.30, 'Sumo');

-- planogram
INSERT INTO planogram
VALUES (1000245245289, 10, 101, 'AutoVending', 2, 15, 'loc'),
       (1000245245289, 11, 101, 'AutoVending', 3, 15, 'loc'),
       (7584367489702, 11, 201, 'iVM Ltd.', 2, 10, 'loc'),
       (3700000736548, 12, 201, 'iVM Ltd.', 2, 10, 'loc'),
       (1000245245289, 11, 301, 'Apple', 3, 10, 'loc'),
       (7584367489702, 11, 401, 'WeAutomate', 3, 20, 'loc'),
       (1273567894358, 12, 401, 'WeAutomate', 3, 10, 'loc'),
       (3700000736548, 10, 501, 'IVM Plus', 2, 15, 'loc'),
       (1000245245289, 11, 501, 'IVM Plus', 2, 15, 'loc'),
       (1000245245289, 10, 601, 'IVMs de Qualidade', 3, 15, 'loc'),
       (7584367489702, 11, 601, 'IVMs de Qualidade', 2, 15, 'loc'),
       (1273567894358, 11, 701, 'iVendors Ltd.', 3, 10, 'loc'),
       (1273567894358, 12, 701, 'iVendors Ltd.', 2, 10, 'loc');

-- retailer
INSERT INTO retailer
VALUES (452, 'Paulo Seixo'),
       (937, 'Amilcar'),
       (762, 'Maria Gertrudes');

-- responsible_for
INSERT INTO responsible_for
VALUES ('Fruta', 452, 101, 'AutoVending'),
       ('Refrigerante', 937, 201, 'iVM Ltd.'),
       ('Fruta', 762, 301, 'Apple'),
       ('Refrigerante', 452, 401, 'WeAutomate'),
       ('Snack', 937, 501, 'IVM Plus'),
       ('Alimentos', 762, 601, 'IVMs de Qualidade'),
       ('Sumo', 452, 701, 'iVendors Ltd.'),
       ('Refrigerante', 452, 702, 'iVendors Ltd.'),
       ('Fruta', 452, 703, 'iVendors Ltd.'),
       ('Snack', 452, 704, 'iVendors Ltd.');

-- replenishment_event
INSERT INTO replenishment_event
VALUES (1000245245289, 10, 101, 'AutoVending', '2022-06-19', 8, 452),
       (1000245245289, 11, 101, 'AutoVending', '2022-06-20', 5, 452),
       (7584367489702, 11, 201, 'iVM Ltd.', '2022-06-20', 4, 937),
       (1000245245289, 11, 301, 'Apple', '2022-06-21', 6, 762),
       (1000245245289, 11, 301, 'Apple', '2022-06-25', 10, 762),
       (7584367489702, 11, 401, 'WeAutomate', '2022-06-22', 7, 452),
       (3700000736548, 10, 501, 'IVM Plus', '2022-06-23', 6, 937),
       (1000245245289, 10, 601, 'IVMs de Qualidade', '2022-06-24', 4, 762),
       (1273567894358, 12, 701, 'iVendors Ltd.', '2022-06-23', 8, 452);