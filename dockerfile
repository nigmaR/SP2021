FROM debian
COPY src . 
RUN chmod ugo+x sp1_18.sh
CMD ./sp1_18.sh
