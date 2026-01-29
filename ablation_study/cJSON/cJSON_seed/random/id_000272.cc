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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    cJSON *meta_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_score = (cJSON *)0;
    double assigned_score = 0.0;
    double retrieved_score_value = 0.0;
    double combined_score = 0.0;
    char json_buf[] = " { \"alpha\": 1, \"beta\": 2 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    score_item = cJSON_CreateNumber(0.0);
    assigned_score = cJSON_SetNumberHelper(score_item, 123.456);

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "score", score_item);

    // step 4: Operate
    cJSON_Minify(json_buf);
    meta_item = cJSON_CreateString(json_buf);
    cJSON_AddItemToObject(root, "meta", meta_item);

    // step 5: Validate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_score = cJSON_GetObjectItem(root, "score");
    retrieved_score_value = cJSON_GetNumberValue(retrieved_score);
    combined_score = assigned_score + retrieved_score_value;
    cJSON_SetNumberHelper(score_item, combined_score);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}