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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char * key_status = "status";
    const char * key_flag = "enabled";
    const char * status_val = "ok";
    cJSON * root = (cJSON *)0;
    cJSON * status_item = (cJSON *)0;
    cJSON * flag_item = (cJSON *)0;
    cJSON_bool has_status = (cJSON_bool)0;
    cJSON_bool has_flag = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a false boolean item
    root = cJSON_CreateObject();
    flag_item = cJSON_CreateFalse();

    // step 3: Configure - add a string to the object and attach the false item
    status_item = cJSON_AddStringToObject(root, key_status, status_val);
    cJSON_AddItemToObject(root, key_flag, flag_item);

    // step 4: Operate - check presence of the items we added
    has_status = cJSON_HasObjectItem(root, key_status);
    has_flag = cJSON_HasObjectItem(root, key_flag);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(status_item != (cJSON *)0) + (int)has_status + (int)has_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}