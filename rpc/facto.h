/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FACTO_H_RPCGEN
#define _FACTO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct reponse {
	float facto;
	int errno;
};
typedef struct reponse reponse;


#define FACTO 0x20000002
#define VERSION_UN 1

#if defined(__STDC__) || defined(__cplusplus)
#define FACTO_NULL 0
extern  void * facto_null_1(void *, CLIENT *);
extern  void * facto_null_1_svc(void *, struct svc_req *);
#define FACTO_RES 1
extern  reponse * facto_res_1(int *, CLIENT *);
extern  reponse * facto_res_1_svc(int *, struct svc_req *);
extern int facto_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define FACTO_NULL 0
extern  void * facto_null_1();
extern  void * facto_null_1_svc();
#define FACTO_RES 1
extern  reponse * facto_res_1();
extern  reponse * facto_res_1_svc();
extern int facto_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_reponse (XDR *, reponse*);
extern  bool_t xdr_reponse (XDR *, reponse*);

#else /* K&R C */
extern bool_t xdr_reponse ();
extern bool_t xdr_reponse ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FACTO_H_RPCGEN */