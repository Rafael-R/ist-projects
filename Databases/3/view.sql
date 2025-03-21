CREATE VIEW sales (ean, category, year, quarter, month, day, weekday, district, county, units) AS
SELECT hc.ean,
       hc.category_name,
       EXTRACT(YEAR FROM re.instant)    AS year,
       EXTRACT(QUARTER FROM re.instant) AS quarter,
       EXTRACT(MONTH FROM re.instant)   AS month,
       EXTRACT(DAY FROM re.instant)     AS day,
       EXTRACT(DOW FROM re.instant)     AS weekday,
       rp.district,
       rp.county,
       re.units
FROM has_category hc
    INNER JOIN replenishment_event re ON re.ean = hc.ean
    INNER JOIN installed_at c ON c.serial_number = re.serial_number 
                              AND c.manufacturer = re.manufacturer
    INNER JOIN retail_point rp ON rp.name = c.location
    INNER JOIN shelf s ON s.number = re.number
                       AND s.serial_number = re.serial_number 
                       AND s.manufacturer = re.manufacturer

WHERE hc.category_name = s.category_name;