#include "misc.hpp"

#include "../sdk/CProtoBufMsgBase.hpp"

#include "../config.hpp"

void Misc::recvMsg(CProtoBufMsgBase *msg) {
  switch (msg->type) {
  case EMSG_WALLET_INFO_UPDATE: {
    const int32_t amount = g_config.fakeWalletBalance.get();
    if (!amount) {
      return;
    }

    const auto body = msg->getBody<CMsgClientWalletInfoUpdate>();
    body->set_has_wallet(true);
    body->set_balance(amount);
    body->set_balance64(amount);

    break;
  }
  case EMSG_PACKAGE_RESTRICTION_INFO: {
    const auto body = msg->getBody<PackageRestrictionResponse>();
    body->isRestricted = false;

    g_pLog->notify("Bypassing restriction for PackageID: %u\n",
                   body->packageID);
    break;
  }
  case EMSG_EMAIL_ADDRESS_INFO: {
    const auto email = g_config.fakeEmail.get();
    if (email.size() < 1) {
      return;
    }

    const auto body = msg->getBody<CMsgClientEmailAddrInfo>();
    body->set_email_address(email);
    body->set_email_is_validated(true);

    break;
  }
  }
}
