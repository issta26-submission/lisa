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
//<ID> 648
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_fmt = NULL;
    char *printed_unfmt = NULL;
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("attached");
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddRawToObject(root, "config", "{\"mode\":true,\"threshold\":3.14}");
    cJSON_AddItemToObject(root, "extra", nested);
    cJSON_AddItemToObject(nested, "sibling", str_item);

    // step 3: Operate / Validate
    printed_fmt = cJSON_Print(root);
    printed_unfmt = cJSON_PrintUnformatted(root);
    printed_len = printed_unfmt ? strlen(printed_unfmt) : 0;
    to_copy = printed_len < 1023 ? printed_len : 1023;
    memcpy(scratch, printed_unfmt ? printed_unfmt : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed_unfmt);
    cJSON_free(printed_fmt);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}