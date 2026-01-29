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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *str = cJSON_CreateString("example");
    cJSON *nested = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateNumber(0.0);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    double set_result = 0.0;
    double pi_val = 0.0;
    const char *name_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "name", str);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "counter", inner);
    cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(inner, 42.0);
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    printed = cJSON_Print(root);
    ((char *)buffer)[0] = printed ? printed[0] : 0;
    (void)set_result;
    (void)pi_val;
    (void)name_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}