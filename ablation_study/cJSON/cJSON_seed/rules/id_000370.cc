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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *minify_buf = (char *)cJSON_malloc(256);
    memset(minify_buf, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *item_pi = NULL;
    cJSON *new_pi = NULL;
    cJSON *replacement = NULL;
    double pi_val = 3.1415;
    double retrieved = 0.0;
    size_t len = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "pi", pi_val);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddNumberToObject(root, "count", 10.0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    len = strlen(printed);
    memcpy(minify_buf, printed, len + 1);
    cJSON_Minify(minify_buf);
    parsed = cJSON_Parse(minify_buf);
    item_pi = cJSON_GetObjectItem(parsed, "pi");
    retrieved = cJSON_GetNumberValue(item_pi);
    replacement = cJSON_CreateNumber(retrieved * 2.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "pi", replacement);
    new_pi = cJSON_GetObjectItem(parsed, "pi");
    retrieved = cJSON_GetNumberValue(new_pi);
    minify_buf[0] = (char)('0' + (int)retrieved);
    minify_buf[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minify_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}