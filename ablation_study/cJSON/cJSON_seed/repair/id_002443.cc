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
//<ID> 2443
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
    cJSON *pi_added = cJSON_AddNumberToObject(child, "pi", 3.14159);
    (void)pi_added;
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "ok", cJSON_CreateTrue());
    cJSON *answer = cJSON_AddNumberToObject(root, "answer", 42.0);
    (void)answer;

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *child_ref = cJSON_GetObjectItem(root, "child");
    cJSON *pi_item = cJSON_GetObjectItem(child_ref, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *pi_copy = cJSON_AddNumberToObject(root, "pi_copy", pi_val);
    buffer[1] = (char)(int)pi_val;
    buffer[2] = pi_copy ? (char)1 : (char)0;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}