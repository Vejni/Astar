library(leaflet)
library(tidyverse)

df <-  read.table("../results/route_haversine_1.txt", skip = 1, header = FALSE, sep = ",", row.names = NULL)
colnames(df) <- c("longitude", "latitude")

leaflet(data = df) %>%
  addTiles() %>%
  addPolylines(data = df, lat = ~latitude, lng = ~longitude,  color = "red")
