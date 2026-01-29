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
//<ID> 2397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", num);
    cJSON *extra = cJSON_CreateString("to_be_moved");
    cJSON_AddItemToObject(root, "to_detach", extra);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';
    cJSON_bool has_detach = cJSON_HasObjectItem(root, "to_detach");
    cJSON_bool has_count = cJSON_HasObjectItem(root, "count");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "to_detach");
    char *old_vs = cJSON_SetValuestring(detached, "changed"); (void)old_vs;
    cJSON_AddItemToArray(arr, detached);
    char *printed2 = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    buffer[1] = printed2 && printed2[0] ? printed2[0] : '\0';
    buffer[2] = (char)(int)(has_detach + has_count);
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}