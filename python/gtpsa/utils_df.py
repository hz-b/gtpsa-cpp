from . import tpsa, ctpsa
from .utils import tpsa_extract_coefficients_to_nrec
import pandas as pd
import numpy as np


def tpsa2df(a_tpsa: tpsa | ctpsa) -> pd.DataFrame:
    """extract tpsa coefficients into a pandas dataframe

    Args:
        a_tpsa: a tpsa object

    Returns:
        a data frame with all

    """
    nrec = tpsa_extract_coefficients_to_nrec(a_tpsa)
    df = pd.DataFrame(nrec).set_index("index")

    # add order column
    order_columns = [item for item in df.columns if item != "value"]
    order = np.sum(df.loc[:, order_columns], axis=1)
    ndf = df.reindex(columns=list(df.columns) + ["order"])
    ndf.loc[:, "order"] = order
    return ndf


__all__ = ["tpsa2df"]
