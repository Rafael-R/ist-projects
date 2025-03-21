-- 1. Qual o nome do retalhista (ou retalhistas) responsáveis pela reposição
-- do maior número de categorias?
SELECT r.name
FROM responsible_for
      INNER JOIN retailer r ON responsible_for.tin = r.tin
GROUP BY r.name
HAVING COUNT(r.tin) = (SELECT MAX(count)
                       FROM (SELECT tin, COUNT(tin) count
                             FROM responsible_for
                             GROUP BY tin) AS tc);

-- 2. Qual o nome do ou dos retalhistas que são responsáveis por todas as
--  categorias simples?
SELECT tin
FROM responsible_for rf
      INNER JOIN simple_category sc ON rf.category_name = sc.category_name
GROUP BY tin
HAVING COUNT(DISTINCT sc.category_name) = (SELECT COUNT(*)
                                           FROM simple_category);

-- 3. Quais os produtos (ean) que nunca foram repostos?
SELECT DISTINCT p.ean
FROM replenishment_event
      INNER JOIN product p ON replenishment_event.ean != p.ean;

-- 4. Quais os produtos (ean) que foram repostos sempre pelo mesmo retalhista?
SELECT de.ean
FROM (SELECT re.ean, COUNT(DISTINCT tin) AS tin_count
      FROM replenishment_event re
      GROUP BY ean) de
WHERE de.tin_count = 1;