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
//<ID> 2447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.14159265);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 10.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "ok", cJSON_CreateTrue());

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_item, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *added_item = cJSON_AddNumberToObject(meta_item, "added", 7.0);
    cJSON *retrieved_added = cJSON_GetObjectItem(meta_item, "added");
    double added_val = cJSON_GetNumberValue(retrieved_added);
    double total = pi_val + count_val + added_val;
    buffer[1] = (char)(int)total;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}