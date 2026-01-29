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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *greet = cJSON_CreateString("hello-world");
    cJSON *nested = cJSON_CreateObject();
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_greet = NULL;
    char *str_val = NULL;
    char *printed = NULL;
    int is_bool_flag = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddStringToObject(nested, "inner", "inner-value");

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_greet = cJSON_GetObjectItem(root, "greeting");
    str_val = cJSON_GetStringValue(retrieved_greet);
    first_char = str_val[0];
    is_bool_flag = (int)cJSON_IsBool(retrieved_flag);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    printed[0] = (char)(printed[0] + (first_char % 8) + (is_bool_flag ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}