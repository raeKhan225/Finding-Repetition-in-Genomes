from application.job.routes import *


# Run the / route when the flask app in opened
@app.route('/')
def home():
    return render_template('home.html')


if __name__ == '__main__':
    app.run(debug=True)
