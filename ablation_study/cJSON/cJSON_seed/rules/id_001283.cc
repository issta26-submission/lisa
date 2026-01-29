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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *pi_item = NULL;
    cJSON *answer_item = NULL;
    cJSON *tmp_item = NULL;
    cJSON_bool add_ref_result = 0;
    double pi_val = 0.0;
    double answer_val = 0.0;
    double combined = 0.0;
    double final_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    answer_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_item);
    cJSON_AddNullToObject(root, "missing");
    add_ref_result = cJSON_AddItemReferenceToObject(root, "answer_alias", answer_item);

    // step 3: Operate / Validate
    tmp_item = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(tmp_item);
    tmp_item = cJSON_GetObjectItem(root, "answer_alias");
    answer_val = cJSON_GetNumberValue(tmp_item);
    combined = pi_val + answer_val;
    cJSON_SetNumberHelper(answer_item, combined);
    final_val = cJSON_GetNumberValue(answer_item);
    (void)add_ref_result;
    (void)final_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}