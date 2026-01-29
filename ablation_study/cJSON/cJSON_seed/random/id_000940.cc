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
//<ID> 940
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *child_key = "child";
    const char *num_key = "num";
    const char *label_key = "label";
    const char *label_val = "compact";
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *retr_child = (cJSON *)0;
    cJSON *retr_label = (cJSON *)0;
    cJSON *retr_num = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *got_label = (char *)0;
    double got_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root and a nested object with items
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);
    label_item = cJSON_CreateString(label_val);

    // step 3: Configure - attach number and string to the child object, then attach child to root
    add_ok1 = cJSON_AddItemToObject(child, num_key, num_item);
    add_ok2 = cJSON_AddItemToObjectCS(child, label_key, label_item);
    cJSON_AddItemToObject(root, child_key, child);

    // step 4: Operate - retrieve the items and use getters to extract native values
    retr_child = cJSON_GetObjectItem(root, child_key);
    retr_label = cJSON_GetObjectItem(retr_child, label_key);
    retr_num = cJSON_GetObjectItem(retr_child, num_key);
    got_label = cJSON_GetStringValue(retr_label);
    got_num = cJSON_GetNumberValue(retr_num);

    // step 5: Validate - compute a simple validation score using the retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(got_label != (char *)0) + (int)(got_num == 42.5);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}