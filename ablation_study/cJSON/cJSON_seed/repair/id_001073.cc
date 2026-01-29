#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *entry1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, entry2);

    // step 2: Configure
    const char *snippet = "{\"id\":123,\"name\":\"refname\"}";
    cJSON *parsed_ref = cJSON_Parse(snippet);
    cJSON_AddItemReferenceToObject(root, "referenced", parsed_ref);

    // step 3: Operate and Validate
    char *out1 = cJSON_Print(root);
    cJSON *roundtrip = cJSON_Parse(out1);
    char *out2 = cJSON_Print(roundtrip);
    cJSON_free(out2);
    cJSON_free(out1);

    // step 4: Cleanup
    cJSON_Delete(roundtrip);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}