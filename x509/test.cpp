#include <stdio.h>
#include <stdlib.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

int generate_certificate(const char* output_filename) {
    X509* x509 = NULL;
    EVP_PKEY* private_key = NULL;
    RSA* rsa_key = NULL;

    // Generate RSA key
    rsa_key = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!rsa_key) {
        fprintf(stderr, "Error generating RSA key.\n");
        return 1;
    }

    // Create private key from RSA key
    private_key = EVP_PKEY_new();
    if (!private_key || !EVP_PKEY_assign_RSA(private_key, rsa_key)) {
        fprintf(stderr, "Error creating private key.\n");
        RSA_free(rsa_key);
        return 1;
    }

    // Create X.509 certificate
    x509 = X509_new();
    if (!x509) {
        fprintf(stderr, "Error creating X.509 certificate.\n");
        EVP_PKEY_free(private_key);
        return 1;
    }

    // Set version to X509v3
    X509_set_version(x509, 2);

    // Set serial number (you may want to use a unique value here)
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    // Set validity period (in this example, valid for 365 days)
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 365 * 24 * 60 * 60);

    // Set subject and issuer names
    X509_NAME* name = X509_NAME_new();
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char*)"My Certificate", -1, -1, 0);
    X509_set_subject_name(x509, name);
    X509_set_issuer_name(x509, name);
    X509_NAME_free(name);

    // Set public key in the certificate
    X509_set_pubkey(x509, private_key);

    // Sign the certificate with the private key
    if (!X509_sign(x509, private_key, EVP_sha256())) {
        fprintf(stderr, "Error signing the certificate.\n");
        X509_free(x509);
        EVP_PKEY_free(private_key);
        return 1;
    }

    // Write the certificate to a file
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file for writing.\n");
        X509_free(x509);
        EVP_PKEY_free(private_key);
        return 1;
    }
    PEM_write_X509(file, x509);

    // Clean up
    fclose(file);
    X509_free(x509);
    EVP_PKEY_free(private_key);

    return 0;
}

int main() {
    // Specify the output filename for the certificate
    const char* output_filename = "my_certificate.pem";

    // Generate the certificate
    if (generate_certificate(output_filename) == 0) {
        printf("Certificate generated successfully: %s\n", output_filename);
    } else {
        fprintf(stderr, "Error generating certificate.\n");
    }

    return 0;
}