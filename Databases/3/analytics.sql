SELECT instant, weekday, county, SUM(s.units)
FROM sales s,
     replenishment_event
WHERE instant BETWEEN '2022-06-20' AND '2022-06-22'
GROUP BY
    ROLLUP (instant, weekday, county)
ORDER BY instant;


SELECT district, county, category, weekday, SUM(units)
FROM sales
GROUP BY
    ROLLUP (district, county, category, weekday)
ORDER BY district;