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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *root_key_child = "child";
    const char *child_key_pi = "pi_obj";
    const char *root_key_answer = "answer";
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_direct = (cJSON *)0;
    cJSON *fetched_answer = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_pi = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and a child object and a number item
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);

    // step 3: Configure - attach child to root, attach number to child, and add a number directly to root
    add_ok1 = cJSON_AddItemToObject(root, root_key_child, child);
    add_ok2 = cJSON_AddItemToObject(child, child_key_pi, num_item);
    num_direct = cJSON_AddNumberToObject(root, root_key_answer, 42.0);

    // step 4: Operate - serialize the structure and then fetch items using both retrieval APIs
    json_str = cJSON_Print(root);
    fetched_answer = cJSON_GetObjectItem(root, root_key_answer);
    fetched_child = cJSON_GetObjectItemCaseSensitive(root, root_key_child);
    fetched_pi = cJSON_GetObjectItem(fetched_child, child_key_pi);

    // step 5: Validate - compute a simple validation score that uses the retrieved results
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(num_direct != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(fetched_answer != (cJSON *)0) + (int)(fetched_child != (cJSON *)0) + (int)(fetched_pi != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}