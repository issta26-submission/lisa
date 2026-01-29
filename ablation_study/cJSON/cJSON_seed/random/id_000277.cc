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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *age_plus_one = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    cJSON *retrieved_age = (cJSON *)0;
    double age_val = 0.0;
    double set_ret = 0.0;
    char *printed = (char *)0;
    char json[] = "{ \"greeting\" : \" Hello World \" , \"age\" : 29 }";

    // step 2: Initialize
    cJSON_Minify(json);
    root = cJSON_CreateObject();

    // step 3: Configure
    str_item = cJSON_CreateString(json);
    num_item = cJSON_CreateNumber(0.0);
    set_ret = cJSON_SetNumberHelper(num_item, 29.0);
    cJSON_AddItemToObject(root, "raw", str_item);
    cJSON_AddItemToObject(root, "age", num_item);

    // step 4: Operate
    retrieved_raw = cJSON_GetObjectItem(root, "raw");
    retrieved_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(retrieved_age);
    age_plus_one = cJSON_CreateNumber(age_val + 1.0);
    cJSON_AddItemToObject(root, "age_plus_one", age_plus_one);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}