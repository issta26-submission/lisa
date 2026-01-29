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
//<ID> 2007
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemToObject(child, "tag", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *shared = cJSON_CreateObject();
    cJSON_AddItemToObject(shared, "label", cJSON_CreateString("shared_label"));
    cJSON_AddItemToObject(root, "shared", shared);
    cJSON_AddItemReferenceToArray(arr, shared);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_num = cJSON_GetObjectItem(got_child, "num");
    double val = cJSON_GetNumberValue(got_num);
    cJSON *got_shared = cJSON_GetObjectItem(root, "shared");
    cJSON *got_label = cJSON_GetObjectItem(got_shared, "label");
    char *label = cJSON_GetStringValue(got_label);
    buffer[0] = (char)('0' + ((int)val % 10));
    buffer[1] = label ? label[0] : 'x';
    buffer[2] = buffer[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}