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
//<ID> 2449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "pi", 3.1415);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "valid", t);
    cJSON_AddItemToObject(root, "data", child);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 7.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *pi_item = cJSON_GetObjectItem(data, "pi");

    // step 3: Operate
    double pi_val = cJSON_GetNumberValue(pi_item);
    buffer[1] = (char)(int)pi_val;
    cJSON_AddNumberToObject(data, "e", 2.718);
    cJSON *e_item = cJSON_GetObjectItem(data, "e");
    double e_val = cJSON_GetNumberValue(e_item);
    buffer[2] = (char)(int)e_val;
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';

    // step 4: Validate and Cleanup
    double count_val = cJSON_GetNumberValue(count_item);
    buffer[4] = (char)(int)count_val;
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}