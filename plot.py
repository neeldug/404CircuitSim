import plotly.express as px
import pandas as pd
import argparse

parser = argparse.ArgumentParser(description="Add Options to Control Plot")
parser.add_argument("file_name", metavar="FILE", type=str, nargs=1,
                    help="File name for SPICE output to plot (.csv formatting)")
parser.add_argument("-c", "--column-names",
                    action='store_true', help="show column names")
parser.add_argument("include", metavar="COLUMN_NAME", type=str, nargs='*',
                    help="Space separated list of columns to plot, if omitted all columns will be plotted", default=None)


if __name__ == '__main__':
    args = parser.parse_args()
    df = pd.read_csv(args.file_name[0]).dropna(axis=1, how='all')
    if args.column_names:
        print(df.columns.values[1:])
        exit(0)
    else:
        column_names = df.columns.values if not args.include else ["Time", *args.include]
        df_vals = pd.melt(df, id_vars=column_names[0], value_vars=column_names[1:])
        fig = px.line(df_vals, x="Time", y="value", color='variable')
        fig.show()