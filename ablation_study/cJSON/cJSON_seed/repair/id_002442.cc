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
//<ID> 2442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "childnum", 7.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(got_flag);
    buffer[1] = (char)(int)flag_true;
    buffer[2] = (char)(int)pi_val;
    cJSON *computed = cJSON_AddNumberToObject(root, "computed", pi_val * 2.0);
    cJSON *got_computed = cJSON_GetObjectItem(root, "computed");
    double computed_val = cJSON_GetNumberValue(got_computed);
    buffer[3] = (char)(int)computed_val;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[4] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}