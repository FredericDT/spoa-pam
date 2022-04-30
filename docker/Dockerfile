FROM library/debian:bullseye AS builder
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
RUN apt-get clean
RUN sed -i -E 's/(deb|security).debian.org/mirrors.bfsu.edu.cn/g' /etc/apt/sources.list
WORKDIR /opt
ADD . /opt/
RUN apt-get update && apt-get install -y libevent-dev libb64-dev libpam-dev libhiredis-dev build-essential
RUN make

FROM library/debian:bullseye
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
RUN apt-get clean
RUN sed -i -E 's/(deb|security).debian.org/mirrors.bfsu.edu.cn/g' /etc/apt/sources.list
RUN apt-get update && apt-get install -y libevent-2.1-7 libb64-0d \
    libpam-radius-auth libhiredis0.14 libevent-pthreads-2.1-7 inetutils-ping \
    && apt-get clean
COPY --from=builder /opt/spoa /usr/sbin/spoa
COPY ./docker/etc/pam.d/fdt-spoa-pam /etc/pam.d/fdt-spoa-pam
COPY ./docker/entrypoint.sh /entrypoint.sh
RUN mkdir /lib/security
RUN chmod u+x /entrypoint.sh
ENV FDT_SPOA_REDIS_HOST=redis
# ENV FDT_SPOA_REDIS_PASS=
ENV FDT_SPOA_REDIS_PORT=6379
ENV FDT_SPOA_PAM_RADIUS_HOST=radius
ENV FDT_SPOA_PAM_RADIUS_SHARED_SECRET=testing123
ENV FDT_SPOA_PAM_RADIUS_TIMEOUT=5
EXPOSE 12345
ENTRYPOINT ["/entrypoint.sh"]