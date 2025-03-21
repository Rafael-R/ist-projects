CREATE OR REPLACE FUNCTION ri1(super varchar(80), sub varchar(80)) RETURNS bool
AS
$$
BEGIN
    RETURN super != sub;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION ri2(_ean numeric, _number numeric, _sn numeric, _man varchar(80),
                               _units numeric) RETURNS bool
AS
$$
DECLARE
    p_units int;
BEGIN
    SELECT units
    INTO p_units
    FROM planogram
    WHERE ean = _ean
      AND number = _number
      AND serial_number = _sn
      AND manufacturer = _man;

    RETURN _units <= p_units;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION ri3(_ean numeric, _number numeric, _sn numeric) RETURNS bool
AS
$$
DECLARE
    common integer;

BEGIN
    SELECT COUNT(*)
    INTO common
    FROM has_category hc
             INNER JOIN shelf s ON s.category_name = hc.category_name
    WHERE hc.ean = _ean
      AND s.number = _number
      AND s.serial_number = _sn;

    RETURN common > 0;
END;
$$ LANGUAGE plpgsql;


-- TRIGGERS

CREATE OR REPLACE FUNCTION insert_category() RETURNS TRIGGER
AS
$$
BEGIN
    INSERT INTO category VALUES (NEW.category_name);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION insert_product() RETURNS TRIGGER
AS
$$
BEGIN
    INSERT INTO has_category VALUES (NEW.ean, NEW.category_name);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;