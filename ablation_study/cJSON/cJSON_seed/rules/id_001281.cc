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
//<ID> 1281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *num_key = "pi";
    const char *ref_key = "pi_ref";
    const char *str_key = "greet";
    const char *null_key = "nothing";
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *null_item = NULL;
    cJSON *tmp = NULL;
    double val = 0.0;
    double ref_val = 0.0;
    cJSON_bool add_item_result = 0;
    cJSON_bool add_ref_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    add_item_result = cJSON_AddItemToObject(root, num_key, num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, str_key, str);
    null_item = cJSON_AddNullToObject(root, null_key);
    add_ref_result = cJSON_AddItemReferenceToObject(root, ref_key, num);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, num_key);
    val = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, ref_key);
    ref_val = cJSON_GetNumberValue(tmp);
    (void)val;
    (void)ref_val;
    (void)add_item_result;
    (void)add_ref_result;
    (void)null_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}