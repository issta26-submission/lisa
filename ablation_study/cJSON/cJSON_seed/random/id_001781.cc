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
//<ID> 1781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *sref = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_s = (cJSON *)0;
    cJSON *retrieved_n = (cJSON *)0;
    cJSON *detached_s = (cJSON *)0;
    cJSON *detached_n = (cJSON *)0;
    char *sval = (char *)0;
    double nval = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    sref = cJSON_CreateStringReference("hello world");
    num = cJSON_CreateNumber(123.456);

    // step 3: Configure
    add_ok1 = cJSON_AddItemReferenceToObject(root, "greeting", sref);
    add_ok2 = cJSON_AddItemReferenceToObject(root, "answer", num);

    // step 4: Operate
    retrieved_s = cJSON_GetObjectItem(root, "greeting");
    sval = cJSON_GetStringValue(retrieved_s);
    retrieved_n = cJSON_GetObjectItem(root, "answer");
    nval = cJSON_GetNumberValue(retrieved_n);

    // step 5: Validate
    validation = (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(sval != (char *)0) + (int)(nval == 123.456);
    (void)validation;

    // step 6: Cleanup
    detached_s = cJSON_DetachItemFromObjectCaseSensitive(root, "greeting");
    detached_n = cJSON_DetachItemFromObjectCaseSensitive(root, "answer");
    cJSON_Delete(root);
    cJSON_Delete(detached_s);
    cJSON_Delete(detached_n);
    (void)root;
    (void)sref;
    (void)num;
    (void)retrieved_s;
    (void)retrieved_n;
    (void)detached_s;
    (void)detached_n;
    (void)sval;
    (void)nval;
    (void)add_ok1;
    (void)add_ok2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}