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
//<ID> 943
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_num = "pi";
    const char *key_str = "greeting";
    const char *str_expected = "example";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    double num_val = 0.0;
    const char *str_val = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create items and add to root (use both AddItemToObject and AddItemToObjectCS)
    num_item = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString(str_expected);
    add_ok1 = cJSON_AddItemToObject(root, key_num, num_item);
    add_ok2 = cJSON_AddItemToObjectCS(root, key_str, str_item);

    // step 4: Operate - extract values using required getters
    num_val = cJSON_GetNumberValue(num_item);
    str_val = cJSON_GetStringValue(str_item);

    // step 5: Validate - compute a simple aggregated score from meaningful data flow
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(num_val > 0.0) + (int)(str_val != (const char *)0);

    // step 6: Cleanup
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}