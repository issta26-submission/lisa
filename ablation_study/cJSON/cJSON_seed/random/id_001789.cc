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
//<ID> 1789
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *holder = (cJSON *)0;
    cJSON *s = (cJSON *)0;
    cJSON *n = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    const char *strval = (const char *)0;
    double numval = 0.0;
    cJSON_bool add_ref_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s = cJSON_CreateString("hello world");
    n = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", s);
    cJSON_AddItemToObject(root, "answer", n);
    holder = cJSON_CreateObject();
    add_ref_ok = cJSON_AddItemReferenceToObject(holder, "ref_greeting", s);

    // step 4: Operate
    detached_num = cJSON_DetachItemFromObjectCaseSensitive(root, "answer");
    numval = cJSON_GetNumberValue(detached_num);
    ref_item = cJSON_GetObjectItem(holder, "ref_greeting");
    strval = cJSON_GetStringValue(ref_item);

    // step 5: Validate
    validation = (int)(detached_num != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(numval == 42.0) + (int)(add_ref_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_num);
    cJSON_Delete(holder);
    cJSON_Delete(root);
    (void)root;
    (void)holder;
    (void)s;
    (void)n;
    (void)detached_num;
    (void)ref_item;
    (void)strval;
    (void)numval;
    (void)add_ref_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}