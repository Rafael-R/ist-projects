#!/usr/bin/python3
from multiprocessing.connection import wait
from wsgiref.handlers import CGIHandler
from flask import Flask, render_template, request, url_for, redirect
import psycopg2
import psycopg2.extras

## SGBD configs
DB_HOST = "db.tecnico.ulisboa.pt"
DB_USER = "ist195665"
DB_DATABASE = DB_USER
DB_PASSWORD = "918273645"
DB_CONNECTION_STRING = "host=%s dbname=%s user=%s password=%s" % (
    DB_HOST,
    DB_DATABASE,
    DB_USER,
    DB_PASSWORD,
)

app = Flask(__name__)


@app.route("/")
def home():
    return render_template("base.html", retailers=retailers)


# ============= CATEGORIES =============

@app.route('/categories')
def categories():
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cursor.execute("SELECT * FROM simple_category;")
    simple_categories = cursor.fetchall()
    cursor.execute("SELECT * FROM super_category;")
    super_categories = cursor.fetchall()
    
    cursor.close()
    dbConn.close()
    return render_template("categories.html", simple_categories=simple_categories, super_categories=super_categories)


@app.route('/categories/create', methods=["POST"])
def create_category():

    name = request.form["name"]

    try:
        dbConn = psycopg2.connect(DB_CONNECTION_STRING)
        cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)
        cursor.execute(f"INSERT INTO simple_category VALUES('{name}')")
   
        cursor.execute("SELECT * FROM simple_category;")
        simple_categories = cursor.fetchall()
        cursor.execute("SELECT * FROM super_category;")
        super_categories = cursor.fetchall()
        return render_template("categories.html", simple_categories=simple_categories, super_categories=super_categories)
    
    except Exception as e:
        return str(e)
    finally:
        dbConn.commit()
        cursor.close()
        dbConn.close()


@app.route('/categories/remove')
def remove_category():

    name = request.args.get("name")
    simple = request.args.get("simple")

    try:
        dbConn = psycopg2.connect(DB_CONNECTION_STRING)
        cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)

        if(simple == 'true'):
            query = f"DELETE FROM simple_category WHERE category_name = '{name}'"
        else:
            query = f"DELETE FROM has_other WHERE super_category_name = '{name}' OR category_name = '{name}'"
            cursor.execute(query)
            query = f"DELETE FROM super_category WHERE category_name = '{name}'"
        
        cursor.execute(query)

        cursor.execute("SELECT * FROM simple_category;")
        simple_categories = cursor.fetchall()
        cursor.execute("SELECT * FROM super_category;")
        super_categories = cursor.fetchall()
        return render_template("categories.html", simple_categories=simple_categories, super_categories=super_categories)


    except Exception as e:
        return str(e)

    finally:
        dbConn.commit()
        cursor.close()
        dbConn.close()

# ============= RETAILER =============

@app.route('/retailers')
def retailers():
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cursor.execute("SELECT tin, name FROM retailer;")
    retailers = cursor.fetchall()
    cursor.close()
    dbConn.close()
    return render_template("retailers.html", retailers=retailers)

@app.route('/retailers/create', methods=["POST"])
def create_retailer():

    tin = request.form["tin"]
    name = request.form["name"]

    try:
        dbConn = psycopg2.connect(DB_CONNECTION_STRING)
        cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)
        cursor.execute(f"INSERT INTO retailer VALUES('{tin}', '{name}')")
        
        cursor.execute("SELECT tin, name FROM retailer;")
        retailers = cursor.fetchall()
        return render_template("retailers.html", retailers=retailers)
    
    except Exception as e:
        return str(e)
    finally:
        dbConn.commit()
        cursor.close()
        dbConn.close()

@app.route('/retailers/remove')
def remove_retailer():

    tin = request.args.get("tin")

    try:
        dbConn = psycopg2.connect(DB_CONNECTION_STRING)
        cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)

        query = f""
        
        cursor.execute(query)

        return retailers()

    except Exception as e:
        return str(e)

    finally:
        dbConn.commit()
        cursor.close()
        dbConn.close()

# ============= EVENTS =============

@app.route('/ivm')
def ivm():
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cursor.execute("SELECT * FROM ivm;")
    ivms = cursor.fetchall()
    cursor.close()
    dbConn.close()
    return render_template("ivm.html", ivms=ivms)

@app.route('/ivm/replenishment')
def ivm_replenishment():

    sn = request.args.get("sn")
    man = request.args.get("man")

    try:
        dbConn = psycopg2.connect(DB_CONNECTION_STRING)
        cursor = dbConn.cursor(cursor_factory=psycopg2.extras.DictCursor)

        query = f"SELECT s.category_name, SUM(re.units)FROM replenishment_event re INNER JOIN shelf s ON re.serial_number = s.serial_number AND re.manufacturer = s.manufacturer AND re.number = s.number WHERE re.serial_number = '{sn}' AND re.manufacturer = '{man}' GROUP BY s.category_name;"
        
        cursor.execute(query)
        events = cursor.fetchall()

        return render_template("replenishment.html", events=events)

    except Exception as e:
        return str(e)

    finally:
        dbConn.commit()
        cursor.close()
        dbConn.close()



CGIHandler().run(app)