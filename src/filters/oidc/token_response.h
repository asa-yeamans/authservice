#ifndef TRANSPARENT_AUTH_SRC_FILTERS_OIDC_TOKEN_RESPONSE_H_
#define TRANSPARENT_AUTH_SRC_FILTERS_OIDC_TOKEN_RESPONSE_H_
#include "absl/types/optional.h"
#include "jwt_verify_lib/jwks.h"
#include "jwt_verify_lib/jwt.h"

namespace transparent_auth {
namespace filters {
namespace oidc {

/**
 * TokenResponse represents a response from a token retrieval request as defined
 * in
 * https://openid.net/specs/openid-connect-core-1_0.html#TokenResponse.
 */
class TokenResponse {
 private:
  google::jwt_verify::Jwt id_token_;

 public:
  TokenResponse(const google::jwt_verify::Jwt &id_token);
  const google::jwt_verify::Jwt &IDToken() const;
};

class TokenResponseParser;
typedef std::shared_ptr<TokenResponseParser> TokenResponseParserPtr;

/**
 * TokenResponseParser provides methods for parsing a raw input stream into a
 * @refitem TokenResponse.
 */
class TokenResponseParser {
 public:
  virtual ~TokenResponseParser(){};
  /**
   * Parse the given token response.
   * @param client_id the expected client_id that should be present in the id_token `aud` field.
   * @param nonce the expected nonce that should be present in the id_token
   * @param raw the raw response to be parsed
   * @return either an empty result indicating an error or a TokenResponse.
   */
  virtual absl::optional<TokenResponse> Parse(const std::string &client_id,
                                              const std::string &nonce,
                                              const std::string &raw) const = 0;
};

/**
 * TokenResponseParser provides methods for parsing a raw input stream into a
 * @refitem TokenResponse.
 */
class TokenResponseParserImpl final : public TokenResponseParser {
 private:
  google::jwt_verify::JwksPtr keys_;

 public:
  TokenResponseParserImpl(google::jwt_verify::JwksPtr keys);
  absl::optional<TokenResponse> Parse(const std::string &client_id,
                                      const std::string &nonce,
                                      const std::string &raw) const override;
};

}  // namespace oidc
}  // namespace filters
}  // namespace transparent_auth
#endif  // TRANSPARENT_AUTH_SRC_FILTERS_OIDC_TOKEN_RESPONSE_H_
