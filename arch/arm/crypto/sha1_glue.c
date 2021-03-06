/*
 * Cryptographic API.
 * Glue code for the SHA1 Secure Hash Algorithm assembler implementation
 *
 * This file is based on sha1_generic.c and sha1_ssse3_glue.c
 *
 * Copyright (c) Alan Smithee.
 * Copyright (c) Andrew McDonald <andrew@mcdonald.org.uk>
 * Copyright (c) Jean-Francois Dive <jef@linuxbe.org>
 * Copyright (c) Mathias Krause <minipli@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/internal/hash.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cryptohash.h>
#include <linux/types.h>
#include <crypto/sha.h>
#include <asm/byteorder.h>
<<<<<<< HEAD

struct SHA1_CTX {
	uint32_t h0,h1,h2,h3,h4;
	u64 count;
	u8 data[SHA1_BLOCK_SIZE];
};

asmlinkage void sha1_block_data_order(struct SHA1_CTX *digest,
=======
#include <asm/crypto/sha1.h>


asmlinkage void sha1_block_data_order(u32 *digest,
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
		const unsigned char *data, unsigned int rounds);


static int sha1_init(struct shash_desc *desc)
{
<<<<<<< HEAD
	struct SHA1_CTX *sctx = shash_desc_ctx(desc);
	memset(sctx, 0, sizeof(*sctx));
	sctx->h0 = SHA1_H0;
	sctx->h1 = SHA1_H1;
	sctx->h2 = SHA1_H2;
	sctx->h3 = SHA1_H3;
	sctx->h4 = SHA1_H4;
=======
	struct sha1_state *sctx = shash_desc_ctx(desc);

	*sctx = (struct sha1_state){
		.state = { SHA1_H0, SHA1_H1, SHA1_H2, SHA1_H3, SHA1_H4 },
	};

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	return 0;
}


<<<<<<< HEAD
static int __sha1_update(struct SHA1_CTX *sctx, const u8 *data,
			       unsigned int len, unsigned int partial)
=======
static int __sha1_update(struct sha1_state *sctx, const u8 *data,
			 unsigned int len, unsigned int partial)
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
{
	unsigned int done = 0;

	sctx->count += len;

	if (partial) {
		done = SHA1_BLOCK_SIZE - partial;
<<<<<<< HEAD
		memcpy(sctx->data + partial, data, done);
		sha1_block_data_order(sctx, sctx->data, 1);
=======
		memcpy(sctx->buffer + partial, data, done);
		sha1_block_data_order(sctx->state, sctx->buffer, 1);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	}

	if (len - done >= SHA1_BLOCK_SIZE) {
		const unsigned int rounds = (len - done) / SHA1_BLOCK_SIZE;
<<<<<<< HEAD
		sha1_block_data_order(sctx, data + done, rounds);
		done += rounds * SHA1_BLOCK_SIZE;
	}

	memcpy(sctx->data, data + done, len - done);
=======
		sha1_block_data_order(sctx->state, data + done, rounds);
		done += rounds * SHA1_BLOCK_SIZE;
	}

	memcpy(sctx->buffer, data + done, len - done);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	return 0;
}


<<<<<<< HEAD
static int sha1_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len)
{
	struct SHA1_CTX *sctx = shash_desc_ctx(desc);
=======
int sha1_update_arm(struct shash_desc *desc, const u8 *data,
		    unsigned int len)
{
	struct sha1_state *sctx = shash_desc_ctx(desc);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	unsigned int partial = sctx->count % SHA1_BLOCK_SIZE;
	int res;

	/* Handle the fast case right here */
	if (partial + len < SHA1_BLOCK_SIZE) {
		sctx->count += len;
<<<<<<< HEAD
		memcpy(sctx->data + partial, data, len);
=======
		memcpy(sctx->buffer + partial, data, len);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
		return 0;
	}
	res = __sha1_update(sctx, data, len, partial);
	return res;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(sha1_update_arm);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83


/* Add padding and return the message digest. */
static int sha1_final(struct shash_desc *desc, u8 *out)
{
<<<<<<< HEAD
	struct SHA1_CTX *sctx = shash_desc_ctx(desc);
=======
	struct sha1_state *sctx = shash_desc_ctx(desc);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	unsigned int i, index, padlen;
	__be32 *dst = (__be32 *)out;
	__be64 bits;
	static const u8 padding[SHA1_BLOCK_SIZE] = { 0x80, };

	bits = cpu_to_be64(sctx->count << 3);

	/* Pad out to 56 mod 64 and append length */
	index = sctx->count % SHA1_BLOCK_SIZE;
	padlen = (index < 56) ? (56 - index) : ((SHA1_BLOCK_SIZE+56) - index);
	/* We need to fill a whole block for __sha1_update() */
	if (padlen <= 56) {
		sctx->count += padlen;
<<<<<<< HEAD
		memcpy(sctx->data + index, padding, padlen);
=======
		memcpy(sctx->buffer + index, padding, padlen);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	} else {
		__sha1_update(sctx, padding, padlen, index);
	}
	__sha1_update(sctx, (const u8 *)&bits, sizeof(bits), 56);

	/* Store state in digest */
	for (i = 0; i < 5; i++)
<<<<<<< HEAD
		dst[i] = cpu_to_be32(((u32 *)sctx)[i]);
=======
		dst[i] = cpu_to_be32(sctx->state[i]);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

	/* Wipe context */
	memset(sctx, 0, sizeof(*sctx));
	return 0;
}


static int sha1_export(struct shash_desc *desc, void *out)
{
<<<<<<< HEAD
	struct SHA1_CTX *sctx = shash_desc_ctx(desc);
=======
	struct sha1_state *sctx = shash_desc_ctx(desc);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	memcpy(out, sctx, sizeof(*sctx));
	return 0;
}


static int sha1_import(struct shash_desc *desc, const void *in)
{
<<<<<<< HEAD
	struct SHA1_CTX *sctx = shash_desc_ctx(desc);
=======
	struct sha1_state *sctx = shash_desc_ctx(desc);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	memcpy(sctx, in, sizeof(*sctx));
	return 0;
}


static struct shash_alg alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_init,
<<<<<<< HEAD
	.update		=	sha1_update,
	.final		=	sha1_final,
	.export		=	sha1_export,
	.import		=	sha1_import,
	.descsize	=	sizeof(struct SHA1_CTX),
	.statesize	=	sizeof(struct SHA1_CTX),
=======
	.update		=	sha1_update_arm,
	.final		=	sha1_final,
	.export		=	sha1_export,
	.import		=	sha1_import,
	.descsize	=	sizeof(struct sha1_state),
	.statesize	=	sizeof(struct sha1_state),
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	.base		=	{
		.cra_name	=	"sha1",
		.cra_driver_name=	"sha1-asm",
		.cra_priority	=	150,
		.cra_flags	=	CRYPTO_ALG_TYPE_SHASH,
		.cra_blocksize	=	SHA1_BLOCK_SIZE,
		.cra_module	=	THIS_MODULE,
	}
};


static int __init sha1_mod_init(void)
{
	return crypto_register_shash(&alg);
}


static void __exit sha1_mod_fini(void)
{
	crypto_unregister_shash(&alg);
}


module_init(sha1_mod_init);
module_exit(sha1_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA1 Secure Hash Algorithm (ARM)");
MODULE_ALIAS("sha1");
MODULE_AUTHOR("David McCullough <ucdevel@gmail.com>");
