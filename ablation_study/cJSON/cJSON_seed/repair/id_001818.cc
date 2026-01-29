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
//<ID> 1818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\":1,\"b\":\"old\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added_number", 42.5);
    cJSON *b_item = cJSON_GetObjectItem(parsed, "b");
    char *set_result = cJSON_SetValuestring(b_item, "updated");
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double a_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "a"));
    double added_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added_number"));
    const char *b_str = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "b"));
    buffer[0] = printed[0];
    buffer[1] = b_str[0];
    int sum = (int)a_val + (int)added_val;
    buffer[2] = (char)('0' + (sum % 10));
    (void)set_result;
    (void)version;
    (void)sum;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}