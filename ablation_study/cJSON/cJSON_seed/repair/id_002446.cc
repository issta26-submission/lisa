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
//<ID> 2446
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddNumberToObject(meta, "count", 42.0);
    cJSON_AddItemToObject(meta, "flag", flag);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "version", 1.2);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_ref, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "computed", count_val + 10.0);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    buffer[2] = (char)(int)ver_val;
    cJSON_AddNumberToObject(meta_ref, "extra", 99.0);

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[1] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}