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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *minbuf = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *pi_item = NULL;
    cJSON *ok_item = NULL;
    cJSON *replacement = NULL;
    double pi_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "one", 1.0);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddTrueToObject(root, "ok");
    cJSON_AddNumberToObject(child, "sub", 42.0);
    cJSON_AddItemToObject(root, "child", child);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    minbuf = (char *)cJSON_malloc(printed_len + 1);
    memcpy(minbuf, printed, printed_len + 1);
    cJSON_Minify(minbuf);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(minbuf);
    pi_item = cJSON_GetObjectItem(parsed, "pi");
    pi_val = cJSON_GetNumberValue(pi_item);
    replacement = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "pi", replacement);
    ok_item = cJSON_GetObjectItem(parsed, "ok");
    (void)ok_item; /* used to indicate validation of presence */

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minbuf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}