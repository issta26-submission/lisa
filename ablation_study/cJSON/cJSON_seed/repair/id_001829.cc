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
//<ID> 1829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *countItem = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(payload, "count", countItem);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *ratioItem = cJSON_AddNumberToObject(payload, "ratio", 2.5);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON *got_count = cJSON_GetObjectItem(got_payload, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *got_ratio = cJSON_GetObjectItem(got_payload, "ratio");
    double ratio_val = cJSON_GetNumberValue(got_ratio);
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'V';
    buffer[2] = label_str ? label_str[0] : 'L';
    buffer[3] = (char)('0' + (((int)count_val) % 10));
    buffer[4] = (char)('0' + (((int)ratio_val) % 10));
    (void)buffer; (void)label_str; (void)count_val; (void)ratio_val; (void)ver; (void)ratioItem;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}