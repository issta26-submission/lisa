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
//<ID> 717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buf1 = (char *)cJSON_malloc(256);
    memset(buf1, 0, 256);
    char *buf2 = (char *)cJSON_malloc(256);
    memset(buf2, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *root_num = NULL;
    cJSON *inner_num = NULL;
    cJSON *true_item = NULL;
    cJSON *got_inner = NULL;
    char *printed = NULL;
    double inner_value = 0.0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    root_num = cJSON_AddNumberToObject(root, "root_value", 42.5);
    child = cJSON_AddObjectToObject(root, "child");
    inner_num = cJSON_AddNumberToObject(child, "inner_value", 3.14159);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    got_inner = cJSON_GetObjectItem(child, "inner_value");
    inner_value = cJSON_GetNumberValue(got_inner);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    memcpy(buf2, printed ? printed : "", printed_len);
    buf2[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buf1);
    cJSON_free(buf2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}